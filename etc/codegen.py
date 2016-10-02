#!/usr/bin/env python
#Generates communications code for the miniboard firmware, based on
#a command protocol specification document (see docparse.py) supplied
#as the first command-line argument.
#
#This code generation system has two goals: to allow commands to be easily added
#to the spec prior to their implementation, and to ensure that all commands
#function consistently. To accomplish this, all command-specific communication
#code is generated automatically from the specification document.
#For this to be possible, the code that actually carries out each command
#must be placed elsewhere. 
#
#Conceptually, commands to the rover read and write registers in its memory.
#This model makes separating communication and action easy. The automatically-generated
#communication code reads and writes fields in a large global struct (also automatically
#generated), with each field corresponding to a command argument.
#
#The output of this program is a source file and a header file containing the auto-generated
#communication code. 
#The global structure is called Data.
#The parse_packet(uint8_t *buf, uint16_t count) function initiates packet
#parsing and dispatches the appropriate reply. buf must contain the command
#byte as the first byte in the buffer. count must be the number of bytes in
#the packet, not including the start or end bytes, and with escape bytes removed.
#
#The host program must provide a send_packet(uint8_t *data, uint16_t count) function
#which adds start/end bytes and escapes special characters, then sends the packet
#over the radio.
#The host program must also provide a memcpy() function.
#
#Additionally, there is an asynchronous command element beyond the simple read/write memory
#model. When a command includes a variable-length argument, a <command name>_trigger(uint8_t *data, uint16_t *len)
#function will be called when the command is received. This function must be implemented by
#the host code. The purpose of this mechanism is to notify the action code when the data
#changes. The trigger function must be capable of being run in an interrupt.
import sys
from docparse import *

def gen_send_proto(cmd_dict):
	"""Given a command dictionary (from extract_table), return the prototype
	of a function that transmits the command."""
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
	
	return comment + proto

def get_all_args(cmd_list):
	"""Return a list of tuples (format code, argument name, note comment)."""
	l = list()
	for c in cmd_list:
		for a in c["argument"]:
			l += [(a[0], a[1], c["notes"])]
	return l
	
def gen_struct_def(cmd_list):
	s = "struct comm_data_t {\n"
	for c in get_all_args(cmd_list):
		dt = format_code_to_cstdint(c[0])
		if "*" in dt:
			#Arrays
			s += "\t" + dt[0:-2] + " " + c[1] + "[%d];"%(2 ** int(dt[4:5])) + " /* " + c[2] + " */\n"
		else:
			s += "\t" + dt + " " + c[1] + ";" " /* " + c[2] + " */\n"
	s += "};\n"
	return s


def gen_header(cmd_list):
	"""Return a string containing the C header for the communication module."""
	s = "#include <stdint.h>\n\n"
	s += gen_struct_def(cmd_list)
	s += "extern struct comm_data_t Data;\n\n"
	#TODO: packet parse function header
	for c in cmd_list:
		s += gen_send_proto(c) + "\n"
	return s
	
def gen_source():
	gen_struct_dec()
	gen_send_func();
	gen_packing();
	gen_parse();
	
def main():
	if len(sys.argv) != 2:
		sys.stderr.write("error: wrong number of arguments. Expected path to spec file as only command line argument.")
	with open(sys.argv[1], "r") as f:
		cmds = extract_table(f.read())
		print gen_header(cmds)

if __name__ == "__main__":
	main()
	