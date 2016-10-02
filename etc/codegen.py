#!/usr/bin/env python
#Generates communications code for the miniboard firmware, based on
#a command protocol specification document (see docparse.py) supplied
#as the first command-line argument.


import sys
from docparse import *

def send_prototype(cmd_dict, write_mode):
	"""Given a command dictionary (from extract_table), return a function that constructs
	   a packet of that type. If write_mode is true, the command is transmitted in the
	   write form; otherwise, the read form is used.
	   
	   The function relies on the following functions:
	     send(uint8_t *data, uint16_t count) - send the given packet across the radio link.
	                                           The send() function must add a start byte and 
	                                           escape start and escape characters where necessary.
	     pack*(void *data, uint16_t pos) - pack the given value (where * = 8, 16, 32, or 64 bits)
	                                       into the given buffer position in a little-endian format.
	     memcpy(void *dest, void *src, uint16_t count) - copy bytes"""
	cmd = cmd_dict
	
	#Comment
	comment = "/* Note: %s */\n"%textwrap.fill(cmd["notes"], 80).replace("\n", "\n * ")
	
	#Prototype
	proto = "void "
	proto += "send_" + cmd["name"].lower().replace(" ", "_") + "("
	for a in cmd["argument"]:
		proto += format_code_to_cstdint(a[0]) + " " + a[1] + ", "
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
			body += "\tpack%d(&%s, buf + b);\n"%(s*8, a[1])
			body += "\tb += %d;\n"%s
			totalsize += s
		prev_a = a
		
	#Send command
	send = "\tsend(buf, b);\n}\n"
		
	#Declarations
	declarations = "\tuint16_t b = 1;\n\tuint8_t buf[%d];\n"%totalsize
	
	return comment + proto + declarations + command + body + send

def main():
	if sys.argv != 2:
		sys.stderr.write("error: wrong number of arguments. Expected path to spec file as only command line argument.")
	with open(sys.argv[1], "r") as f:
		cmds = extract_table(f.read())
		for c in cmds:
			print c["argument"]
			print send_prototype(c, False)

if __name__ == "__main__":
	main()
	