/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gps.c - Ublox Neo-7M GPS module interface.
 * Author(s): Nick Ames
 */
#include "gps.h"
#include "uart.h"
#include "commgen.h"
#include <string.h>

#define GPS_UART 2
#define GPS_BAUD 9600

void gps_byte_handler(uint8_t c);
void (*UART2RXHandler)(uint8_t) = gps_byte_handler;

/* Start the GPS listener.
 * Once started, it will update the appropriate fields in the Data
 * struct as GPS data is received. */
void gps_init(void){
	uart_enable(GPS_UART, GPS_BAUD, 1, 0);
}

#define GPS_BUF_LEN 128
static uint8_t GPSBuf[GPS_BUF_LEN];
static uint8_t GPSBufSize;

/* Parse a buffer of comma-separated fields. 
 * Ftable will be populated with the indexes of the
 * first character in each field. The number of fields
 * extracted is returned. */
static uint8_t parse_table(uint8_t *buf, uint8_t buflen, uint8_t *ftable, uint8_t tablesize){	
	return 1;
}

/* Handle a GLL (Latitude and Longitude) message. */
static void handle_gll(uint8_t *buf, uint8_t size){
	
}

/* Check a string against a memory buffer.
 * Returns 1 if the string matches the beginning of the buffer,
 * 0 otherwise. */
static uint8_t match(const char *str, const uint8_t *buf){
	int l = strlen(str);
	if(memcmp(str, buf, l) == 0){
		return 1;
	} else {
		return 0;
	}
}

/* Handle an entire NMEA line. This function is 
 * called by gps_byte_handler() when a newline is encountered. */
static void handle_nmea(){
	if(match("$GPGLL", GPSBuf))handle_gll(GPSBuf, GPSBufSize);
}

/* Handle each byte as it comes into the UART. */
void gps_byte_handler(uint8_t c){
	if(GPSBufSize < GPS_BUF_LEN){
		GPSBuf[GPSBufSize] = c;
		GPSBufSize++;
	}
	if(c == '\n'){
		handle_nmea();
		GPSBufSize = 0;
	}
}