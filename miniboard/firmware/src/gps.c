/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * gps.c - Ublox Neo-7M GPS module interface.
 * Author(s): Nick Ames
 */
#include <avr/interrupt.h>
#include "gps.h"
#include "uart.h"
#include "commgen.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

#define GPS_BUF_LEN 100
#define GPS_VALID_TIMEOUT 3 /* Number of seconds before the GPS fix is marked invalid. Max. 4. */
static uint8_t GPSBuf[GPS_BUF_LEN];
static uint8_t GPSBufSize;

/* Start a countdown timer for GPS_VALID_TIMEOUT seconds.
 * Once it expires, the Data->gps_valid field will be set to 0. */
static void start_gps_valid_timer(void){
	TCCR5B = 0;
	TCNT5 = 0;
	OCR5A = (uint16_t) 15625*GPS_VALID_TIMEOUT;
	TIMSK5 = _BV(OCIE5A);
	TCCR5B = _BV(CS52) | _BV(CS50); /* Set 1024 prescaler. */
}

ISR(TIMER5_COMPA_vect){
	Data->gps_pos_valid = 0;
	Data->gps_track_valid = 0;
	TCCR5B = 0;
}

/* Parse a buffer of comma-separated fields. 
 * Ftable will be populated with the indexes of the
 * first character in each field. The number of fields
 * extracted is returned. */
static uint8_t parse_table(const uint8_t *buf, uint8_t buflen, uint8_t *ftable, uint8_t tablesize){	
	uint8_t i;
	if(tablesize == 0 || buflen == 0)return 0;
	ftable[0] = 0;
	uint8_t t = 1;
	uint8_t prev_comma = 0;
	for(i = 0; i < 255 && buf[i] != '\n'; i++){
		if(prev_comma){
			prev_comma = 0;
			ftable[t] = i;
			t++;
		}
		if(buf[i] == ','){
			prev_comma = 1;
			continue;
		}
	}
	return t-1;
}

/* Extract a latitude from a field (ten characters, including
 * decimal point). The field must have a comma at the end.
 * The result is in 10e-6 minute units, and will always be positive.
 * On success, 0 is returned. On failure, 1 is returned. */
static uint8_t extract_lat(const uint8_t *field, int64_t *result){
	/* Determine if there are 10 characters in the field,
	 * and copy them into a local buffer, removing the
	 * decimal point and adding a terminating null. */
	char dbuf[3], mbuf[8];
	uint8_t mb = 0;
	for(int i=0;i < 11;i++){
		if(field[i] == ','){
			if(i != 10){
				return 1;
			} else {
				break;
			}
		}
		if(i == 10 && field[i] != ','){
			return 1;
		}
		if(field[i] == '.'){
			if(i == 4){
				continue;
			} else {
				return 1;
			}
		}
		if(!isdigit(field[i])){
			return 1;
		} else {
			if(i == 0 || i == 1){
				dbuf[i] = field[i];
			} else {
				mbuf[mb] = field[i];
				mb++;
			}
		}	
	}
	dbuf[2] = '\0';
	mbuf[mb] = '\0';
	*result = (atoi(dbuf) * 60 * 100000) + atoi(mbuf);
	return 0;
}

/* Extract a longitude from a field (eleven characters, including
 * decimal point). The field must have a comma at the end.
 * The result is in 10e-6 minute units, and will always be positive.
 * On success, 0 is returned. On failure, 1 is returned. */
static uint8_t extract_long(const uint8_t *field, int64_t *result){
	/* Determine if there are 10 characters in the field,
	 * and copy them into a local buffer, removing the
	 * decimal point and adding a terminating null. */
	char dbuf[4], mbuf[8];
	uint8_t mb = 0;
	for(int i=0;i < 12;i++){
		if(field[i] == ','){
			if(i != 11){
				return 1;
			} else {
				break;
			}
		}
		if(i == 11 && field[i] != ','){
			return 1;
		}
		if(field[i] == '.'){
			if(i == 5){
				continue;
			} else {
				return 1;
			}
		}
		if(!isdigit(field[i])){
			return 1;
		} else {
			if(i <= 2){
				dbuf[i] = field[i];
			} else {
				mbuf[mb] = field[i];
				mb++;
			}
		}	
	}
	dbuf[3] = '\0';
	mbuf[mb] = '\0';
	*result = (atoi(dbuf) * 60 * 100000) + atoi(mbuf);
	return 0;
}

/* Extract a decimal number from a comma-terminated field.
 * The whole-number part will be placed in whole_result.
 * The fractional part will be placed in fractional_result.
 * The number of digits in the fractional part will be placed
 * in fractional_digits, allowing the size of each part in
 * facrtional_result to be determined. On success, 0 is returned.
 * On failure, >0 is returned. */
static uint8_t extract_decimal(const uint8_t *field, int32_t *whole_result, uint32_t *fractional_result, uint8_t *fractional_digits){
	uint8_t flen;
	uint8_t wlen; /* Whole-number portion length. */
	uint8_t dlen; /* Decimal portion length. */
	uint8_t decimal_found = 0;
	for(flen = 0; flen < 255 && field[flen] != ','; flen++){
		/* Determine the number of characters in the field. */
		if(field[flen] == '.'){
			if(decimal_found)return 1; /* More than one decimal point. */
			decimal_found = 1;
			wlen = flen;
		} else if(!isdigit(field[flen]) && field[flen] != '-'){
			return 2; /* Non-number character in field. */
		}
	}
	if(flen == 0)return 3;
	if(decimal_found){
		char wbuf[wlen + 1];
		memcpy(wbuf, field, wlen);
		wbuf[wlen] = '\0';
		dlen = flen - wlen - 1;
		char dbuf[dlen + 1];
		memcpy(dbuf, field + wlen + 1, dlen);
		dbuf[dlen] = '\0';
		*whole_result = atoi(wbuf);
		*fractional_result = atoi(dbuf);
		*fractional_digits = dlen;
	} else {
		/* No fractional part. */
		char wbuf[flen + 1];
		memcpy(wbuf, field, flen);
		*whole_result = atoi(wbuf);
		*fractional_result = 0;
		*fractional_digits = 0;
	}
	
	return 0;
}

/* Raise base to a power. */
uint32_t ipow(uint32_t base, uint32_t power){
	if(power == 0)return 1;
	uint32_t result = base;
	for(;power > 1;power--){
		result *= base;
	}
	return result;
}

/* Extract a decimal from a comma-terminated field.
 * Unit determines the scale of the whole-number and fractional portion.
 * If precision = 0, the result has the same scale as the whole-number portion.
 * The fractional portion will be ignored.
 * If precision = 1, the result has a scale of .1. The whole-number portion will be
 * multiplied by 10. If the fractional portion has more digits (such as .1002), they
 * will be ignored.
 * On success, 0 is returned.
 * On failure, >0 is returned. */
static uint8_t extract_decimal_precision(const uint8_t *field, uint8_t precision, int32_t *result){
	uint32_t frac;
	int32_t whole;
	uint8_t r, fdigits;
	r = extract_decimal(field, &whole, &frac, &fdigits);
	if(r != 0){
		return r;
	}
	if(precision > fdigits){
		frac *= ipow(10, precision - fdigits);
	} else {
		frac /= ipow(10, fdigits - precision);
	}
	
	*result = whole * ipow(10, precision) + frac;
	return 0;
}

/* Handle a GGA (GPS data) message. */
static void handle_gga(uint8_t *buf, uint8_t size){
	int64_t latitude, longitude;
	int32_t altitude;
	uint8_t ftable[15];
	if(14 != parse_table(buf, size, ftable, 15))return;
	
	if(extract_lat(buf + ftable[2], &latitude))return;
	if(*(buf + ftable[3]) != 'N' && *(buf + ftable[3]) != 'S')return;
	if(*(buf + ftable[3]) == 'S')latitude = -latitude;
	
	if(extract_long(buf + ftable[4], &longitude))return;
	if(*(buf + ftable[5]) != 'W' && *(buf + ftable[5]) != 'E')return;
	if(*(buf + ftable[5]) == 'W')longitude = -longitude;
	
	if(*(buf + ftable[6]) == '0')return; /* No fix. */
	
	if(extract_decimal_precision(buf + ftable[9], 1, &altitude))return;
	Data->latitude = latitude;
	Data->longitude = longitude;
	Data->altitude = altitude;
	Data->gps_pos_valid = 1;
	start_gps_valid_timer();
}

/* Handle a VTG (course over ground) message. */
static void handle_vtg(uint8_t *buf, uint8_t size){
	int32_t heading, speed;
	uint8_t ftable[15];
	if(9 != parse_table(buf, size, ftable, 15))goto error;
	if(*(buf + ftable[9]) != 'A')goto error;
	if(extract_decimal_precision(buf + ftable[1], 2, &heading))goto error;
	if(extract_decimal_precision(buf + ftable[7], 3, &speed))goto error;
	Data->gps_heading = heading;
	Data->gps_speed = speed;
	start_gps_valid_timer();
	return;
	
	error:
	/* Invalidate the gps track immediately, separately from the gps position,
	 * since the receiver can provide position information without giving a track. */
	Data->gps_track_valid = 0;
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
static void handle_nmea(uint8_t *buf, uint8_t size){
	if(match("$GPGGA", buf))handle_gga(buf, size);
	if(match("$GPVTG", buf))handle_vtg(buf, size);
}

/* Handle each byte as it comes into the UART. */
void gps_byte_handler(uint8_t c){
	if(GPSBufSize < GPS_BUF_LEN){
		GPSBuf[GPSBufSize] = c;
		GPSBufSize++;
	}
	if(c == '\n'){
		handle_nmea(GPSBuf, GPSBufSize);
		GPSBufSize = 0;
	}
}