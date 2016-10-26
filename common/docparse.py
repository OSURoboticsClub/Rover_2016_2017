#!/usr/bin/env python
#Parses a markdown file (first command line argument)
#containing a table with the header whose header starts with:
#| Name | RW | Command Code
import textwrap
import sys
import math

def extract_table(file_str):
	"""Extract the command table from the text of
	   specification.md. Returns a list of dictionaries with the following
	   members:
	     "name"     : str, name of command
	     "rw"       : str, contains r is command can be read, w if command can be written, - if neither
	     "code"     : int, command code (0-127)
	     "argument" : list( #list of 0 or more tuples, each with the following structure:
	       tuple(format code:str, #command format code, u8, i16, etc, or * to indicate a
	                              #a variable number of bytes controlled by the previous argument.
	             name:str))
	     "default"  : list of default values of arguments, as strings
	     "notes"    : str, notes on command."""
	s = file_str.find("\n| Name | RW | Command Code")
	s+=1 #Skip over first newline
	e = file_str[s:].find("\n\n")
	if e < s:
		table_str = file_str[s:]
	else:
		table_str = file_str[s:s + e]
	commands = []
	for l in table_str.split("\n"):
		if len(l) == 0: continue
		if l[0] == "|":
			l = l[1:]
		col = [c.strip() for c in l.split("|")]
		if col[0][0:3] == "---": continue
		if col[0] == "Name": continue
		name = col[0]
		rw = col[1].lower()
		code = int(col[2], 0)
		notes = col[5]
		argument = []
		for a in [c.strip() for c in col[3].split(",")]:
			a = a.split(" ")
			if len(a) == 2:
				argument.append((a[0], a[1]))
		default = []
		if len(argument) > 0 and col[4].strip() != "-":
			for d in [c.strip() for c in col[4].split(",")]:
				default.append(d)
		if len(default) != len(argument) and len(default) != 0:
			sys.stderr.write("warning: default list length mismatch on command '%s'\n"%name)
		commands.append({
			"name" : name,
			"rw"   : rw,
			"code" : code,
			"argument" : argument,
			"default"  : default,
			"notes" : notes})
	return commands

def format_code_to_cstdint(fmt_name):
	"""Convert a format code (i8, u16, *, etc) to a c standard int (int8_t, uint16_t, uint8_t *)."""
	if fmt_name == "*":
		return "uint8_t *"
	elif fmt_name[0] == "i":
		return "int%s_t"%fmt_name[1:]
	elif fmt_name[0] == "u":
		return "uint%s_t"%fmt_name[1:]
	else:
		raise ValueError("Unknown format code \"%s\""%fmt_name)

def format_code_size(fmt_name):
	"""Return the number of bytes required for a type with the given format code."""
	if fmt_name == "*":
		return 255
	else:
		return int(fmt_name[1:])/8

def cannon_name(command):
	"""Given the name of a command, return the canonical function name
	   (lowercase, with spaces converted to underscores)."""
	return command.lower().replace(" ", "_")

def gen_send_proto(cmd_dict):
	"""Given a command dictionary (from extract_table), return the prototype
	of a function that transmits the command."""
	cmd = cmd_dict
	
	#Comment
	comment = "/* Note: %s */\n"%textwrap.fill(cmd["notes"], 80).replace("\n", "\n * ")
	
	#Prototype
	need_trigger = False
	proto = "void "
	proto += "send_" + cannon_name(cmd["name"]) + "("
	for a in cmd["argument"]:
		proto += "volatile " + format_code_to_cstdint(a[0]) + " " + a[1] + ", "
		if a[0] == "*": need_trigger = True
	proto = proto[0:-2] #Remove last commma 
	proto += ");\n"
	
	#Trigger
	trigger = ""
	if need_trigger:
		trigger = "void " + cannon_name(cmd["name"]) + "_trigger(void);\n"
	
	return comment + proto + trigger

def gen_send_func(cmd_dict, write_mode):
	"""Given a command dictionary (from extract_table()), return a function that constructs
	   a packet of that type. If write_mode is true, the command is transmitted in the
	   write form; otherwise, the read form is used.
	   
	   The function relies on the following functions:
	     send_packet(uint8_t *data, uint16_t count) - send the given packet across the radio link.
	                                                  The send_packet() function must add a start and end byte and 
	                                                  escape characters where necessary.
	     pack*(uint8_t *data, uint16_t pos, uint*_t value) - pack the given value (where * = 8, 16, 32, or 64 bits)
	                                       into the given buffer position in a little-endian format.
	     memcpy(void *dest, void *src, uint16_t count) - copy bytes"""
	cmd = cmd_dict
	
	#Comment
	comment = "/* Note: %s */\n"%textwrap.fill(cmd["notes"], 80).replace("\n", "\n * ")
	
	#Prototype
	proto = "void "
	proto += "send_" + cannon_name(cmd["name"]) + "("
	for a in cmd["argument"]:
		proto += "volatile " + format_code_to_cstdint(a[0]) + " " + a[1] + ", "
	proto = proto[0:-2] #Remove last commma 
	proto += "){\n"
	
	#Command
	command = "\tbuf[0] = 0x%X;\n"%(cmd["code"] | (int(write_mode) << 7))
	
	#Packet stuffing
	body = ""
	totalsize = 1 #current byte in packet buffer
	prev_a = None
	for a in cmd["argument"]:
		if a[0] == "*":
			if prev_a == None:
				raise ValueError("In command %s, variable-length argument used before length controlling argument.")
			body += "\tmemcpy(buf + b, %s, %s);\n"%(a[1], prev_a[1])
			body += "\tb += %s;\n"%prev_a[1]
			totalsize += 255
		else:
			s = format_code_size(a[0])
			body += "\tpack%d(buf, b, %s);\n"%(s*8, a[1])
			body += "\tb += %d;\n"%s
			totalsize += s
		prev_a = a
		
	#Send command
	send = "\tsend_packet(buf, b);\n}\n"
		
	#Declarations
	declarations = "\tuint16_t b = 1;\n\tuint8_t buf[%d];\n"%totalsize
	
	return comment + proto + declarations + command + body + send

def gen_parse_proto(cmd_dict):
	"""Given a command dictionary (from extract_table()), return a function that
	   Extracts a packet of that type from a buffer. The first byte in the buffer
	   should be the command byte, escape characters must be removed, and it need
	   not contain the end byte.
	   
	   The generated function relies on the following functions:
	     unpack*(uint8_t *data, uint16_t pos, uint*_t *result) - (where * = 8, 16, 32, etc.) 
	                                                       unpack the little-endian value from the buffer and write it
	                                                       to a variable.
	     memcpy(void *dest, void *src, uint16_t count) - copy bytes"""
	cmd = cmd_dict
	
	#Comment
	comment = "/* Note: %s */\n"%textwrap.fill(cmd["notes"], 80).replace("\n", "\n * ")
	
	#Prototype
	proto = "void "
	proto += "parse_" + cannon_name(cmd["name"]) + "("
	proto += "uint8_t *packet, "
	for a in cmd["argument"]:
		typename = "volatile " + format_code_to_cstdint(a[0]).strip()
		if typename[-1] == "*":
			typename = typename[0:-1]
		proto += typename + " *" + a[1] + ", "
	proto = proto[0:-2] #Remove last commma 
	proto += ";\n"
	
	return comment + proto

def gen_parse_func(cmd_dict):
	"""Given a command dictionary (from extract_table()), return a function that
	   Extracts a packet of that type from a buffer. The first byte in the buffer
	   should be the command byte, escape characters must be removed, and it need
	   not contain the end byte.
	   
	   The generated function relies on the following functions:
	     unpack*(uint8_t *data, uint16_t pos, uint*_t *result) - (where * = 8, 16, 32, etc.) 
	                                                       unpack the little-endian value from the buffer and write it
	                                                       to a variable.
	     memcpy(void *dest, void *src, uint16_t count) - copy bytes"""
	cmd = cmd_dict
	
	#Comment
	comment = "/* Note: %s */\n"%textwrap.fill(cmd["notes"], 80).replace("\n", "\n * ")
	
	#Prototype
	proto = "void "
	proto += "parse_" + cannon_name(cmd["name"]) + "("
	proto += "uint8_t *packet, "
	for a in cmd["argument"]:
		typename = "volatile " + format_code_to_cstdint(a[0]).strip()
		if typename[-1] == "*":
			typename = typename[0:-1]
		proto += typename + " *" + a[1] + ", "
	proto = proto[0:-2] #Remove last commma 
	proto += "){\n"
	
	#Packet stuffing
	body = ""
	totalsize = 1 #current byte in packet buffer
	prev_a = None
	for a in cmd["argument"]:
		if a[0] == "*":
			if prev_a == None:
				raise ValueError("In command %s, variable-length argument used before length controlling argument.")
			body += "\tmemcpy((void *) %s, packet + b, *%s);\n"%(a[1], prev_a[1])
			body += "\tb += %s;\n"%prev_a[1]
			totalsize += 255
		else:
			s = format_code_size(a[0])
			body += "\tunpack%d(packet, b, (uint%d_t *) %s);\n"%(s*8, s*8, a[1])
			body += "\tb += %d;\n"%s
			totalsize += s
		prev_a = a
		
	#Declarations
	declarations = "\tuint16_t b = 1;\n"
	
	return comment + proto + declarations + body + "}\n"

def gen_packing_protos():
	"""Return a string containing the prototypes of the functions:
	      unpack*(uint8_t *data, uint16_t pos, uint*_t *result) - (where * = 8, 16, 32, etc.) 
	                                                       unpack the little-endian value from the buffer and write it
	                                                       to a variable.
	      pack*(uint8_t *data, uint16_t pos, uint*_t value) - pack the given value (where * = 8, 16, 32, or 64 bits)
	                                                          into the given buffer position in a little-endian format.
	   where * = 8, 16, 32, and 64."""
	s = ""
	
	for n in [8, 16, 32, 64]:
		s += "void pack%d(volatile uint8_t *data, uint16_t pos, uint%d_t value);\n"%(n,n)
	
		s += "void unpack%d(volatile uint8_t *data, uint16_t pos, volatile uint%d_t *result);\n"%(n,n)
	return s

def gen_packing_funcs():
	"""Return a string containing the source code to the functions:
	      unpack*(uint8_t *data, uint16_t pos, uint*_t *result) - (where * = 8, 16, 32, etc.) 
	                                                       unpack the little-endian value from the buffer and write it
	                                                       to a variable.
	      pack*(uint8_t *data, uint16_t pos, uint*_t value) - pack the given value (where * = 8, 16, 32, or 64 bits)
	                                                          into the given buffer position in a little-endian format.
	   where * = 8, 16, 32, and 64."""
	s = ""
	
	for n in [8, 16, 32, 64]:
		s += "void pack%d(volatile uint8_t *data, uint16_t pos, uint%d_t value){\n"%(n,n)
		for i in range(0, n/8):
			s += "\t*(data + pos + %d) = (value >> %d) & 0xFF;\n"%(i,i*8)
		s += "}\n"
		
		s += "void unpack%d(volatile uint8_t *data, uint16_t pos, volatile uint%d_t *result){\n"%(n,n)
		s += "\t*result = "
		for i in range(0, n/8):
			s += "(((uint%d_t) *(data + pos + %d) << %d)) | "%(n,i, i * 8)
		s = s[0:-3] #trim last " | "
		s += ";\n}\n"
	return s

if __name__ == "__main__":
	import sys
	with open(sys.argv[1], "r") as f:
		cmds = extract_table(f.read())
		for c in cmds:
			print c["argument"]
