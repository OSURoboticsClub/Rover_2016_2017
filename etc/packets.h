#ifndef PACKETS_H
#define PACKETS_H

#include <stdio.h>
#include <stdint.h>

/* typedefs and constants */

typedef uint8_t byte;

const byte ERROR = 0x00;
const byte START = 0x01;
const byte ESCAPE = 0x02;

/* command codes */
enum commandCodes {
    PAUSE = 0x05,
    BATTERY_VOLTAGE = 0x06,
    MOTOR_POWER = 0x10,
    SWERVE_STATE = 0x11,
    SELECT_STATE = 0x20,
    CAMERA_COMMAND = 0x30,
};

/* library function headers */
void readStream(FILE *input);

void writeStream(FILE *output);

void readStruct(FILE *input, byte type);

// TODO: writeStruct

/* packet type definitions */
namespace packets{

typedef uint16_t batteryVoltage;

enum pauseState {PAUSED=0x00, UNPAUSED=0x01};

struct motorPower{
    int8_t motorLeft;
    int8_t motorRight;
};

enum swerveState{OFF=0x00, STRAIGHT=0x01, TURN=0x02};


enum cameraState{FIRST_CAMERA=0x00, SECOND_CAMERA=0x01,
    THIRD_CAMERA=0x02, FOURTH_CAMERA=0x03
};

/* packet default values */
static struct motorPower MOTOR_POWER_DEFAULT = {0, 0};

}; // packets
#endif // PACKETS_H
