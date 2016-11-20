#!/usr/bin/env python

'''This packet generates C files for handling packets on the 
side of the base station.'''

import codegen as cg

def write_header(file_name, cmd_list):
    '''This function will generate the header file
    for the packet library used for the base station.'''
    with open(file_name, "w+") as fh:
        fh.write(cg.gen_header(cmd_list))

    
def write_source(file_name, cmd_list):
    '''This function writes the source file that will
    be compiled to form the packet library used for
    the base station.'''
    with open(file_name, "w+") as fs:
        fs.write(cg.gen_source(cmd_list))
                                                                    

if __name__ == "__main__":
    import argparse
    import os.path
    parser = argparse.ArgumentParser()
    parser.add_argument("--src", type=str, default=".")
    parser.add_argument("--dest", type=str, default=".")
    parser.add_argument("--spec", type=str, default="SPECIFICATION.md")
    args = parser.parse_args()
    header_file = os.path.join(args.dest, "commgen.h")
    source_file = os.path.join(args.dest, "commgen.c")
    spec_file = os.path.join(args.src, args.spec)
    with open(spec_file) as f:
        messages = cg.extract_table(f.read())

    write_header(header_file, messages)
    write_source(source_file, messages)
