#!/usr/bin/env python
#Parses a markdown file (first command line argument)
#containing a table with the header whose header starts with:
#| Name | RW | Command Code

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
		if len(argument) > 0:
			for d in [c.strip() for c in col[5].split(",")]:
				default.append(d)
		commands.append({
			"name" : name,
			"rw"   : rw,
			"code" : code,
			"argument" : argument,
			"default"  : default,
			"notes" : notes})
	return commands

	
if __name__ == "__main__":
	import sys
	with open(sys.argv[1], "r") as f:
		print extract_table(f.read())
		








