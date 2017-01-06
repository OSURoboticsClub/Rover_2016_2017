#!/usr/bin/env python3

"""This packet generates C++ files for handling packets on the
side of the base station."""
import re

class BasePackets(object):
    def __init__(self, specification=None, header_template=None,
                 source_template=None, header_output="p.h",
                 source_output="packethandler.cpp"):
        self.packets = None
        self.specification = specification
        self.header_template = header_template
        self.source_template = source_template
        self.header_output = header_output
        self.source_output = source_output

        self._arg_size_extractor = re.compile(r"(\d|\*)+$")
        self._tabsize = 4

    def write_header(self):
        if self.specification is None:
            raise ValueError("The specification file has not been specified.")
        elif self.header_template is None:
            raise ValueError("Header template is not specified.")
        elif self.header_output is None:
            raise ValueError("Header output is not specified.")

        if self.packets is None:
            self.extract_table()

        with open(self.header_template, "r") as f:
            header_template = f.read().splitlines()

        with open(self.header_output, "w+") as fh:
            for line in header_template:
                fh.write(line)
                fh.write("\n")
                if "@packet_types_header" in line:
                    fh.write(self.packet_types_header(line=line))
                elif "@recieved_signals_header" in line:
                    fh.write(self.packet_signals_header(line=line))
                elif "@write_slots_header" in line:
                    fh.write(self.write_slots_header(line=line))
                elif "@read_slots_header" in line:
                    fh.write(self.read_slots_header(line=line))


    def write_source(self):
        if self.specification is None:
            raise ValueError("The specification file has not been specified.")
        elif self.source_template is None:
            raise ValueError("Source template is not specified.")
        elif self.source_output is None:
            raise ValueError("Source output is not specified.")

        if self.packets is None:
            self.extract_table()

        with open(self.source_template, "r") as f:
            source_template = f.read().splitlines()

        with open(self.source_output, "w+") as fh:
            for line in source_template:
                fh.write(line)
                fh.write("\n")
                if "@parse_read_cases" in line:
                    fh.write(self.parse_read_cases(line=line))
                elif "@parse_write_cases" in line:
                    fh.write(self.parse_write_cases(line=line))
                elif "@write_slots_source" in line:
                    fh.write(self.write_slots_source(line=line))
                elif "@read_slots_source" in line:
                    fh.write(self.read_slots_source(line=line))

    def extract_table(self):
        with open(self.specification, "r") as f:
            file_str = f.read()

        s = file_str.find("\n| Name | RW | Command Code")
        s += 1  # Skip over first newline
        e = file_str[s:].find("\n\n")
        if e < s:
            table_str = file_str[s:]
        else:
            table_str = file_str[s:s + e]
        self.packets = []
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
                raise RuntimeError(
                    "Default list length mismatch on command %s" % name
                )
            self.packets.append({
                "name": name,
                "rw": rw,
                "code": code,
                "argument": argument,
                "default": default,
                "notes": notes})


    def packet_types_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self.packets:
            uppercase_name = packet["name"].upper().replace(" ", "_")
            string += ws + ("%s = %i,\n" % (uppercase_name, packet["code"]))
        return string

    def packet_signals_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self.packets:
            if "r" in packet["rw"]:

                string += (ws + "void " + self._get_signal_name(packet) + "(" +
                          self._argument_proto(packet) + ");\n")
        return string

    def write_slots_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self.packets:
            if "w" in packet["rw"]:
                string += (ws + "void write" + self._camelcase(packet["name"]) +
                           "(" + self._argument_proto(packet) + ");\n")
        return string

    def read_slots_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self.packets:
            if "r" in packet["rw"]:
                string += (ws + "void read" + self._camelcase(packet["name"]) +
                           "();\n")
        return string

    def parse_read_cases(self, line=None):
        string = ""
        ws = self._whitespace(line)
        i = 0
        for packet in self.packets:
            uppercase_name = packet["name"].upper().replace(" ", "_")
            if "r" not in packet["rw"]:
                continue

            if i is 0:
                string += (ws + "if(packet.at(0) == m_types::%s) {\n"
                           % uppercase_name)
                i += 1
            else:
                string += (ws + "else if(packet.at(0) == m_types::%s) {\n"
                           % uppercase_name)
            packet_size = self._get_packet_size(packet)
            if packet_size is not None:
                string += (ws + "\tif(packet.size() != %i) return;\n" %
                           packet_size)
                for arg in pacet["argument"]:
                    arg_type = self._expand_argument(arg[0])
                    string += ws + "\t%s %s;\n" % (arg_type, arg[1])
                    string += ws + "\tstream >> %s;\n" % arg[1]

                string += ws + "\temit " + self._get_signal_name(packet) + "("
                string += "".join(map(lambda x: x[1] + ", ",
                                      packet["argument"])).strip(" ,")
                string += ");\n"

            string += ws + "}\n"

        return string.expandtabs(self._tabsize)

    def _get_signal_name(self, packet):
        return (self._camelcase(packet["name"], capitalize_first_word=False) +
                "Received")

    def parse_write_cases(self, line=None):
        string = ""
        ws = self._whitespace(line)
        return string

    def write_slots_source(self, line=None):
        string = ""
        ws = self._whitespace(line)
        return string

    def read_slots_source(self, line=None):
        string = ""
        ws = self._whitespace(line)
        return string

    def _expand_argument(self, arg):
        if arg == "*":
            arg_type = "quint8"
        elif "u" in arg:
            arg_type = "q" + arg.replace("u", "uint")
        elif "i" in arg:
            arg_type = "q" + arg.replace("i", "int")
        else:
            raise ValueError("Argument type %s id not recognized" % arg)
        return arg_type

    def _argument_proto(self, packet):
        argument_str = ""
        for arg in packet["argument"]:
            arg_type = self._expand_argument(arg[0])
            argument_str += arg_type + " " + arg[1] + ", "

        argument_str = argument_str.strip(" ,")
        return argument_str

    def _camelcase(self, string, capitalize_first_word=True):
        words = string.split(" ")
        words = map(lambda x: x.capitalize(), words)
        words = "".join(words)
        if not capitalize_first_word:
            words = words[0].lower() + words[1:]
        return words

    def _whitespace(self, reference):
        if reference is None:
            return ""
        num_spaces = (len(reference) - len(reference.lstrip(" ")))
        return " " * num_spaces

    def _get_packet_size(self, packet):
        size = 0
        for arg in packet["argument"]:
            arg_size = re.search(self._arg_size_extractor, arg[0]).group(0)
            if arg_size is None:
                raise ValueError("Cannot determine size of packet.")
            elif arg_size == "*":
                return None
            arg_size = int(arg_size)
            if (arg_size % 8 is not 0):

                raise ValueError("Argument sizes must be multiples of 8.")
            size += arg_size
        return size


if __name__ == "__main__":
    import argparse
    import os.path

    parser = argparse.ArgumentParser()
    parser.add_argument("--src", type=str, default=".")
    parser.add_argument("--dest", type=str, default=".")
    parser.add_argument("--spec", type=str, default="SPECIFICATION.md")
    args = parser.parse_args()

    spec_file = os.path.join(args.src, args.spec)

    b = BasePackets(specification="SPECIFICATION.md",
                    source_template="packethandler.cpp", source_output="p.cpp")
    b.write_source()
