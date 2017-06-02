/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * sbus.c - Futaba S-BUS receiver module.
 * Author(s): Nick Ames
 */

#include <stdint.h>
#include <string.h>
#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "sabertooth.h"
#include "uart.h"
#include "commgen.h"
#include "s-bus.h"

#define SBUS_PACKET_LENGTH 25
#define SBUS_START_BYTE 0x0F
#define SBUS_END_BYTE 0x00

#define SBUS_FLAGS_INDEX 23
#define SBUS_FLAG_FAILSAFE 3

#define SBUS_NUM_CHANNELS 16

void sbus_byte_handler(uint8_t b);
void (*UART3RXHandler)(uint8_t) = sbus_byte_handler;

static uint8_t packet_buffer[SBUS_PACKET_LENGTH];
static uint8_t buffer_current;

static uint16_t sbus_channels[SBUS_NUM_CHANNELS];
static uint8_t sbus_active;

void sbus_init(void) {
	/* Two stop bits + even parity */
	uart_enable(SBUS_UART, SBUS_BAUD, 2, 2);

	/* Reset packet buffer */
	buffer_current = 0;

	/* Reset sbus channel values and set failsafe until first packet */
	memset(sbus_channels, 0, SBUS_NUM_CHANNELS);
	sbus_active = 0;
}

void sbus_release(void) {
	uart_disable(SBUS_UART);
}

/* FRSky channels */
#define JOY_LV 1
#define JOY_LH 4
#define JOY_RV 2
#define JOY_RH 3
#define POT_L 6 /* Pot on front face of FRsky */
#define POT_R 7 /* Pot on front face of FRsky */
#define SIDE_L 5 /* Pot on side of FRsky */
#define SIDE_R 8 /* Pot on side of FRsky */
#define SA 9
#define SB 10
#define SC 11
#define SD 12
#define SWE 13 /* SE conflicts with AVR-libc definition */
#define SF 14
#define SG 15
#define SH 16

/* Channel functions */
#define MODE_SWITCH SWE
#define TURN_SWITCH SG
#define PAUSE_SWITCH SF
#define DRIVE_LEFT JOY_LV
#define DRIVE_RIGHT JOY_RV
#define ARM_BASE JOY_LH
#define ARM_BICEP JOY_LV
#define ARM_FOREARM JOY_RV
#define ARM_PITCH JOY_RH
#define ARM_GRABBER SIDE_L
#define ARM_EE SIDE_R
#define CAMERA_SELECT SH
#define PAN POT_R
#define TILT POT_L
#define ARM_DRILL SIDE_L

/* Convert a switch channel value to a position. */
typedef enum {SW_FORWARD = 1, SW_MIDDLE = 0, SW_BACK = 2} switch_t;
static switch_t switch_ch(uint8_t ch){
	uint16_t servo_value = sbus_channels[ch-1];
	if(servo_value < 300){
		return SW_FORWARD;
	} else if (servo_value >= 300 && servo_value < 1300){
		return SW_MIDDLE;
	} else {
		return SW_BACK;
	}
}

/* Convert a joystick/analog channel value to a motor
 * power value from -127 to 127. */
static int8_t joy_ch(uint8_t ch){
	int32_t servo_value = sbus_channels[ch-1];
	const int16_t center = 1030;
	const int16_t deadband = 40;
	const int16_t amp = 1811 - center - deadband;
	servo_value -= center;
	if(servo_value < deadband && servo_value > -deadband){
		return 0;
	}
	if(servo_value > 0){
		servo_value -= deadband;
	} else {
		servo_value += deadband;
	}
	servo_value *= 127;
	servo_value /= amp;
	if(servo_value > 127){
		servo_value = 127;
	}
	if(servo_value < -127){
		servo_value = -127;
	} 
	return servo_value;
}

void reset_timeout_timer(void);

/* Set values in the Data structure according to s-bus channels, allowing the
 * frsky controller to operate the rover. */
static void sbus_control(void){
	static uint8_t drop_packet;
	static uint8_t prev_active;
	static uint8_t prev_pause;
	static uint8_t prev_cam_select;
	uint8_t cam_select;
	
	if(drop_packet == 2){
		drop_packet = 0;
		return;
	} else {
		drop_packet++;
	}
	
	if(sbus_active && !prev_active){
		/* Got controller */
		prev_pause = Data->pause_state;
		Data->pause_state = 1;
	}
	if(!sbus_active && prev_active){
		/* Lost controller */
		Data->pause_state = prev_pause;
		Data->l_f_drive = 0;
		Data->l_m_drive = 0;
		Data->l_b_drive = 0;
		Data->r_f_drive = 0;
		Data->r_m_drive = 0;
		Data->r_b_drive = 0;
		Data->arm_motor_1 = 0;
		Data->arm_motor_2 = 0;
		Data->arm_motor_3 = 0;
		Data->arm_motor_4 = 0;
		Data->arm_motor_5 = 0;
	}
	prev_active = sbus_active;
	
	Data->sbus_1 = sbus_channels[0];
	Data->sbus_2 = sbus_channels[1];
	Data->sbus_3 = sbus_channels[2];
	Data->sbus_4 = sbus_channels[3];
	Data->sbus_5 = sbus_channels[4];
	Data->sbus_6 = sbus_channels[5];
	Data->sbus_7 = sbus_channels[6];
	Data->sbus_8 = sbus_channels[7];
	Data->sbus_9 = sbus_channels[8];
	Data->sbus_10 = sbus_channels[9];
	Data->sbus_11 = sbus_channels[10];
	Data->sbus_12 = sbus_channels[11];
	Data->sbus_13 = sbus_channels[12];
	Data->sbus_14 = sbus_channels[13];
	Data->sbus_15 = sbus_channels[14];
	Data->sbus_16 = sbus_channels[15];
	Data->sbus_active = sbus_active;
	if(sbus_active){
		if(switch_ch(PAUSE_SWITCH) == SW_FORWARD){
			Data->pause_state = 1;
		} else {
			Data->pause_state = 0;
		}
		Data->pan_speed = -joy_ch(PAN);
		Data->tilt_speed = joy_ch(TILT);
		cam_select = switch_ch(CAMERA_SELECT) == SW_BACK;
		if(cam_select && !prev_cam_select){
			Data->selected_camera++;
			if(Data->selected_camera > 6){
				Data->selected_camera = 1;
			}
		}
		prev_cam_select = cam_select;
		if(switch_ch(MODE_SWITCH) == SW_FORWARD){
			/* Mode 1 - Drive */
			int8_t left = joy_ch(JOY_LV);
			int8_t right = joy_ch(JOY_RV);
			Data->l_f_drive = left;
			Data->l_m_drive = left;
			Data->l_b_drive = left;
			Data->r_f_drive = right;
			Data->r_m_drive = right;
			Data->r_b_drive = right;
			Data->swerve_state = switch_ch(TURN_SWITCH);
			Data->arm_mode = 0;
		} else {
			/* Mode 2 - Arm */
			Data->arm_motor_1 = joy_ch(ARM_BASE);
			Data->arm_motor_2 = -joy_ch(ARM_BICEP);
			Data->arm_motor_3 = joy_ch(ARM_FOREARM);
			Data->arm_motor_5 = 0;
			Data->ee_speed = -8*joy_ch(ARM_EE);
			if(switch_ch(MODE_SWITCH) == SW_MIDDLE){
				/* Arm mode 1 - Grabber */
				Data->arm_mode = 1;
				Data->grabber_rotation_speed = 8*joy_ch(ARM_PITCH);
				Data->grabber_speed = 8*joy_ch(ARM_GRABBER);
			} else {
				/* Arm mode 2 - Container Sealer */
				Data->arm_mode = 2;
				Data->arm_motor_4 = joy_ch(ARM_DRILL);
			}
		}
	}
}

/* Extract an 11-bit, LSB-first, unsigned value from a bit stream.
 * stream is a uint8_t pointer to the stream of bytes.
 * bit_index is the bit number (starting at 0) of the first bit in
 * the value to be extracted. */
uint16_t extract_11b(uint8_t *stream, uint16_t bit_index){
	const uint16_t bil = bit_index;
	const uint16_t biu = bit_index + 10;
	if(((bil/8)+1) != ((biu)/8)){
		/* Value spans three bytes. */
		uint16_t lbyte = stream[bil/8];
		uint16_t mbyte = stream[(bil/8)+1];
		uint16_t ubyte = stream[biu/8];
		uint16_t umask = ((1 << ((biu%8)+1))-1);
		uint16_t lmask = ~((1 << ((bil%8)+1))-1);
		ubyte &= umask;
		lbyte &= lmask;
 		return (lbyte >> (bil%8)) | (mbyte << (8-(bil%8))) | (ubyte << (16-(bil%8)));
	} else {
		/* Value spans two bytes. */
		uint16_t lbyte = stream[bil/8];
		uint16_t ubyte = stream[biu/8];
		uint16_t umask = ((1 << ((biu%8)+1))-1);
		ubyte &= umask;
 		return (lbyte >> (bil%8)) | (ubyte << (8-(bil%8)));
	}
}

/* Handle S-BUS packets */
void sbus_handle_packet(void) {
	/* Verify packet integrity */
	if (packet_buffer[0]                      != SBUS_START_BYTE ||
		packet_buffer[SBUS_PACKET_LENGTH - 1] != SBUS_END_BYTE) {
		return;
	}
	
	sbus_active = !(packet_buffer[SBUS_FLAGS_INDEX] & _BV(SBUS_FLAG_FAILSAFE));
	
	for(int i=0;i<16;i++){
		sbus_channels[i] = extract_11b(packet_buffer + 1, i * 11);
	}
	
	sbus_control();
	reset_timeout_timer();
}

/* Recieve S-BUS protocol bytes as they come in over the UART. When a full
 * packet has been recieved, validate it and handle it. */
void sbus_byte_handler(uint8_t b){
	/* If the expected start and end bytes haven't been recieved, discard the
	 * packet. */
	if (buffer_current == 0 && b != SBUS_START_BYTE) {
		buffer_current = 0;
	} else if (buffer_current == (SBUS_PACKET_LENGTH - 1) && b != SBUS_END_BYTE) {
		buffer_current = 0;
	} else { /* Data byte is okay */
		packet_buffer[buffer_current] = b;
		buffer_current += 1;
	}

	/* If a full packet has been received, reset buffer_current and call the
	 * packet handling function. */
	if (buffer_current >= SBUS_PACKET_LENGTH){
		buffer_current = 0;
		sbus_handle_packet();
	}
}
