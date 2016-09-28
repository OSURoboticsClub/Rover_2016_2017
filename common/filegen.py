#!/usr/bin/env python3
import re

def parse_protobuf(file_name):
    find_messages = re.compile("message \w+")
    with open(file_name, "r+") as fp:
        messages = find_messages.findall(fp.read())
    for i, e in enumerate(messages):
        messages[i] = e.split(" ")[1]
    return messages
    
def write_header(file_name, messages):
    with open(file_name, "w+") as fh:
        fh.write(
            '#ifndef IO_GEN_H\n' +
            '#define IO_GEN_H\n\n' +
            '#include <pb_decode.h>\n\n' +
            '#include "packets.pb.h"\n' +
            '#include "io.h"\n\n\n' +
            '#ifdef __cplusplus\n' + 
            'extern "C" {\n' +
            '#endif\n\n'
        )
        for i, e in enumerate(messages):
            fh.write("#define %s_TYPE %i\n" % (e.upper(), i))
        
        fh.write(    
            '\n' +
            'void messageFields(' +
                'uint8_t type, ' +
                'pb_field_t **fieldptr, ' +
                'size_t *numfields, ' +
                'size_t *maxsize);\n\n'
            )
        for e in messages:
            message_struct = "packets_%s" % (e)
            fh.write((
                '%s decode%s(uint8_t *string, uint8_t type);\n\n') % (
                    message_struct, e
                )
            )
                     
        fh.write(
            '#ifdef __cplusplus\n' +
            '}\n' +
            '#endif // extern "C"\n\n' +
            '#endif // IO_GEN_H\n'
        )
    
def write_source(file_name, messages):
    with open(file_name, "w+") as fs:
        fs.write('#include "io_gen.h"\n\n\n')
        fs.write(
            'void messageFields(' +
                'uint8_t type, ' +
                'pb_field_t **fieldptr, ' +
                'size_t *numfields, ' +
                'size_t *maxsize)\n' +
             '{\n' +
             '  switch(type)\n' +
             '  {\n'
         )
        for e in messages:
            message_type = "%s_TYPE" % (e.upper())
            message_fields = "packets_%s_fields" % (e)
            message_size = "packets_%s_size" % (e)
            fs.write(( 
                '   case %s :\n' +      
                '       *fieldptr = (pb_field_t *)%s;\n' +
                '       *numfields = sizeof(%s) / sizeof(pb_field_t);\n' +
                '       *maxsize = %s;\n' +
                '       break;\n') % (
                    message_type,
                    message_fields,
                    message_fields,
                    message_size
                )
            )            
        fs.write(
            '  }\n' +
            '}\n\n'
        )
        for e in messages:
            message_struct = "packets_%s" % (e)
            message_fields = "%s_fields" % (message_struct)
            fs.write((
                '%s decode%s(uint8_t *string, uint8_t type)\n' +
                '{\n' +
                '   size_t size = 0;\n' +
                "   do{size++;}while(*(string + size) != '\\0');\n" +
                '   pb_istream_t stream = pb_istream_from_buffer((++string), size);\n' +
                '   static %s message;\n' +
                '   pb_decode(&stream, %s, &message);\n' +
                '   return message;\n' +
                '}\n\n') % (
                    message_struct,
                    e,
                    message_struct,
                    message_fields
                )
            )
            

if __name__ == "__main__":
    import argparse
    import os.path
    parser = argparse.ArgumentParser()
    parser.add_argument("--src", type=str, default=".")
    parser.add_argument("--dest", type=str, default=".")
    args = parser.parse_args()
    proto_file = os.path.join(args.src, "packets.proto")
    header_file = os.path.join(args.dest, "io_gen.h")
    source_file = os.path.join(args.dest, "io_gen.c")
    messages = parse_protobuf(proto_file)
    write_header(header_file, messages)
    write_source(source_file, messages)
