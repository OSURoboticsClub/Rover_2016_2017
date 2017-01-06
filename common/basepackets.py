#!/usr/bin/env python3

"""This packet generates C++ files for handling packets on the
side of the base station."""
import re
from collections import OrderedDict

class BasePackets(object):
    def __init__(self, specification=None, files={}):
        # files {"input_file": "output_file"}
        self._packets = None
        self._specification = specification
        self.files = files

        self._params = {}
        self._tabsize = 4

        # requires removing all whitespace
        self._param_extractor = re.compile(r"(?<=:)[a-zA-Z0-9_]+")

        self._class_extractor = re.compile(r"(?<=class )[a-zA-Z]+")
        self._function_arg_extractor = re.compile(r"\(.+\)")
        self._arg_size_extractor = re.compile(r"(\d|\*)+$")

    def packets():
        def fget(self):
            return self._packets
        return locals()
    packets = property(**packets())

    def specification():
        def fget(self):
            return self._specification
        def fset(self, value):
            self._specification = value
        return locals()
    specification = property(**specification())

    def files():
        def fget(self):
            return self._files
        def fset(self, value):
            self._files = OrderedDict(sorted(value.items(),
                key=lambda x: x[0].endswith(".cpp")))
        return locals()
    files = property(**files())

    def write_files(self):
        for t, s in self._files.iteritems():
            self._write_file(str(t), str(s))

    def _write_file(self, template, output):
        if self._specification is None:
            raise ValueError("The specification file has not been specified.")
        if self._packets is None:
            self.extract_table()

        with open(template) as f:
            file_template = f.read().splitlines()

        with open(output, "w+") as f:
            for line in file_template:
                f.write(line + "\n")
                if "class" in line:
                    if "enum" in line:
                        continue
                    self._params.update({"class":
                        re.search(self._class_extractor, line).group(0)})
                if "@" not in line:
                    continue
                elif "@datastream" in line:
                    self._params.update({"datastream":
                        self._extract_param(line)})
                elif "@types_enum" in line:
                    self._params.update({"types_enum":
                        self._extract_param(line)})
                elif "@crc" in line:
                    self._params.update({"crc":
                        self._extract_param(line)})
                elif "@packet_types_header" in line:
                    f.write(self.packet_types_header(line=line))
                elif "@recieved_signals_header" in line:
                    f.write(self.packet_signals_header(line=line))
                elif "@write_slots_header" in line:
                    f.write(self.write_slots_header(line=line))
                elif "@read_slots_header" in line:
                    f.write(self.read_slots_header(line=line))
                elif "@parse_packets" in line:
                    f.write(self.parse_packets(line=line))
                elif "@write_slots_source" in line:
                    f.write(self.write_slots_source(line=line))
                elif "@read_slots_source" in line:
                    f.write(self.read_slots_source(line=line))

    def extract_table(self):
        with open(self._specification, "r") as f:
            file_str = f.read()

        s = file_str.find("\n| Name | RW | Command Code")
        s += 1  # Skip over first newline
        e = file_str[s:].find("\n\n")
        if e < s:
            table_str = file_str[s:]
        else:
            table_str = file_str[s:s + e]
        self._packets = []
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
            self._packets.append({
                "name": name,
                "rw": rw,
                "code": code,
                "argument": argument,
                "default": default,
                "notes": notes})


    def packet_types_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self._packets:
            uppercase_name = packet["name"].upper().replace(" ", "_")
            string += ws + ("%s = %i,\n" % (uppercase_name, packet["code"]))
        return string

    def packet_signals_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self._packets:
            if "r" in packet["rw"]:

                string += (ws + "void " + self._signal_name(packet) + "(" +
                          self._argument_proto(packet) + ");\n")
        return string

    def write_slots_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self._packets:
            if "w" in packet["rw"]:
                string += (ws + "void write" + self._camelcase(packet["name"]) +
                           "(" + self._argument_proto(packet) + ");\n")
        return string

    def read_slots_header(self, line=None):
        string = ""
        ws = self._whitespace(line)
        for packet in self._packets:
            if "r" in packet["rw"]:
                string += (ws + "void read" + self._camelcase(packet["name"]) +
                           "();\n")
        return string

    def parse_packets(self, line=None):
        ws = self._whitespace(line)
        string = ""
        string += ws + "quint16 _crc;\n"
        string += ws + ("%s >> _crc;\n" % self._params["datastream"])
        string += ws + "quint8 _packetType;\n"
        string += ws + ("%s >> _packetType;\n" % self._params["datastream"])
        i = 0
        for packet in self._packets:
            uppercase_name = packet["name"].upper().replace(" ", "_")
            if "r" not in packet["rw"]:
                continue

            if i is 0:
                string += (ws + "if(_packetType == static_cast<quint8>" +
                    "(%s::%s)) {\n" % (
                        self._params["types_enum"],
                        uppercase_name)
                    )
                i += 1
            else:
                string += (ws + "else if(_packetType == static_cast<quint8>" +
                    "(%s::%s)) {\n" % (
                        self._params["types_enum"],
                        uppercase_name)
                    )
            packet_size = self._packet_size(packet)
            if packet_size is not None:
                for arg in packet["argument"]:
                    arg_type = self._expand_argument(arg[0])
                    string += ws + "\t%s %s;\n" % (arg_type, arg[1])
                    string += ws + "\t%s >> %s;\n" % (
                        self._params["datastream"], arg[1]
                    )

                string += ws + "\temit " + self._signal_name(packet) + "("
                string += "".join(map(lambda x: x[1] + ", ",
                                      packet["argument"])).strip(" ,")
                string += ");\n"

            string += ws + "}\n"

        return string.expandtabs(self._tabsize)


    def write_slots_source(self, line=None):
        string = ""
        for packet in self._packets:
            if "w" not in packet["rw"]:
                continue
            # TODO: CRC
            string += ("void %s::write%s (%s)\n{\n" % (
                self._params["class"],
                self._camelcase(packet["name"]),
                self._argument_proto(packet),
            ))
            string += "\t" + self._params["datastream"]
            for arg in packet["argument"]:
                string += " << " + arg[1]
            string += ";\n}\n\n"
        return string.expandtabs(self._tabsize)

    def read_slots_source(self, line=None):
        string = ""
        ws = self._whitespace(line)
        return string

    def _parse_function_args(self, str):
        arguments = re.search(self._function_arg_extractor, str).group(0)
        if arguments is None:
            return None
        arguments = arguments.strip("() ")
        arguments = list(map(lambda x: x.strip(" ,"), arguments))
        return arguments

    def _signal_name(self, packet):
        return (self._camelcase(packet["name"], capitalize_first_word=False) +
                "Received")

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

    def _packet_size(self, packet):
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

    def _extract_param(self, string):
        s = string.replace(" ", "")
        return re.search(self._param_extractor, s).group(0)


if __name__ == "__main__":
    import argparse
    import os.path

    parser = argparse.ArgumentParser()
    parser.add_argument("--src", type=str, default=".")
    parser.add_argument("--dest", type=str, default=".")
    parser.add_argument("--name", type=str, default="packethandler")
    parser.add_argument("--spec", type=str, default="SPECIFICATION.md")
    args = parser.parse_args()
    header_template = os.path.join(args.src, args.name) + ".h"
    header_output = os.path.join(args.dest, args.name) + ".h"
    source_template = os.path.join(args.src, args.name) + ".cpp"
    source_output = os.path.join(args.dest, args.name) + ".cpp"
    specification = args.spec
    b = BasePackets(
        specification=specification,
        files = {header_template: header_output, source_template: source_output}
    )
    b.write_files()
