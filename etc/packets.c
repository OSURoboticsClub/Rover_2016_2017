#include "packets.h"


void readStream(FILE *input){
    char c;
    void *read;

    while(c != EOF){
        c = getc(input);
        if(c == START){
            byte type = (byte)getc(input);
            read = readStruct(&input, type);
        };
    };
};

void *readStruct(FILE *input, byte type){
    void *read;
    switch(type){
        case PAUSE:
            read* = getc(input);
            break;
        case BATTERY_VOLTAGE:
            break;
    };
};
