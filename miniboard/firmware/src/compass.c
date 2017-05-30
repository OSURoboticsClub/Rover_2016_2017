/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * compass.c - Compass Module.
 * Author(s): Zachary Homen
 */
#include <avr/interrupt.h>
#include <stdint.h>
#include "compass.h"
#include "uart.h"
#include "commgen.h"
#include "math.h"
#include <util/atomic.h>

//start condition transmitted
#define TW START 0x08
//repeated start condition transmitted
#define TW_REP_START 0x10
//SLA+W transmitted, ACK received
#define TW_MT_SLA_ACK 0x18
//SLW_W transmitted, NACK received
#define TW_MT_SLA_NACK 0x20

#define COMP_UART 3
#define DEV_ADDRESS 0x1E
#define WR_CMD 0x3C
#define RD_CMD 0x3D
#define DEV_WRITE (DEV_ADDRESS | WR_CMD) //LSB is a zero to write
#define DEV_READ (DEV_ADDRESS | RD_CMD) //LSB is a one to read
#define COMP_VALID_TIMEOUT 2
#define CONF_A 0x00
#define	CONF_B 0x01
#define	MODE 0x02
#define X_MSB 0x03
#define X_LSB 0x04
#define	Z_MSB 0x05
#define	Z_LSB 0x06
#define	Y_MSB 0x07
#define Y_LSB 0x08
#define PI 3.14159

void comp_init(void) {
	twi_init();
	config_rega();
	config_regb();
	mode();
}

static void start_comp_valid_timer(void){
	TCCR2B = 0;
	TCNT2 = 0;
	OCR2A = (uint16_t) 15625*COMP_VALID_TIMEOUT;
	TIMSK2 = _BV(OCIE2A);
	TCCR2B = _BV(CS22) | _BV(CS20); /* Set 1024 prescaler. */
}

ISR(TIMER2_COMPA_vect){
	Data->compass_heading_valid = 0;
}

void compass_retrieve(){
	int16_t x = read_x();
	read_z();
	int16_t y = read_y();
	int16_t heading, val, y1, x1;

	heading = atan2(-y,-x)*(180/PI);
	if(heading < 0){
		heading = heading + 360;
	}
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		Data->compass_heading = heading;
		Data->compass_heading_valid = 1;
	}
	start_comp_valid_timer();
}

void config_rega()
{
	uint8_t confa = 0x70;
	twi_writeto(CONF_A, confa);
}

void config_regb()
{
	uint8_t confb = 0xA0;
	twi_writeto(CONF_B, confb);
}

void mode()
{
	uint8_t mode = 0x00;
	twi_writeto(MODE, mode);
}

int16_t read_x()
{
	uint8_t xh = twi_readfr(X_MSB);
	uint8_t xl = twi_readfr(X_LSB);
	uint16_t headingx = xl | (xh << 8);

	Data->mag_x = headingx;
	return headingx;
}

void read_z()
{
	uint8_t zh = twi_readfr(Z_MSB);
	uint8_t zl = twi_readfr(Z_LSB);

	Data->mag_z = zl | (zh << 8);
}

int16_t read_y()
{
	uint8_t yh = twi_readfr(Y_MSB);
	uint8_t yl = twi_readfr(Y_LSB);
	uint16_t headingy = yl | (yh << 8);

	Data->mag_y = headingy;
	return headingy;
}


/*****************************************************************
I2C
*****************************************************************/

void twi_init(void)
{
    //set SCL to 400kHz
    TWSR = 0x00;
    TWBR = 0x0C;
    //enable TWI
    TWCR = (1<<TWEN);
}

void twi_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}
//send stop signal
void twi_stop(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

uint8_t twi_getStatus(void)
{
    uint8_t status;
    //mask status
    status = TWSR & 0xF8;
    return status;
}

void twi_write(uint8_t u8data)
{
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0){};
}

uint8_t twi_readACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}
//read byte with NACK
uint8_t twi_readNACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t twi_writeto(uint16_t u16addr, uint8_t u8data)
{
    twi_start();
    if (twi_getStatus() != 0x08){
        return 1;
    }
    //select device and send A2 A1 A0 address bits
    twi_write(0x3C);
    if (twi_getStatus() != 0x18){  
        return 1;
    }
    //send the rest of address
    twi_write((uint8_t)(u16addr));
    if (twi_getStatus() != 0x28){
        return 1;
    }
    //write byte to eeprom
    twi_write(u8data);
    if (twi_getStatus() != 0x28){
        return 1;
    }
    twi_stop();
    return 0;
}

uint8_t twi_readfr(uint16_t u16addr) 
{
	uint8_t data;
    //uint8_t databyte;
    twi_start();
    if (twi_getStatus() != 0x08){
        return 1;
    }
    //select devise and send A2 A1 A0 address bits
    twi_write(0x3C);
    if (twi_getStatus() != 0x18){
        return 1;
    }
    //send the rest of address
    twi_write((uint8_t)(u16addr));
    if (twi_getStatus() != 0x28){
        return 1;
    }
    //send start
    twi_start();
    if (twi_getStatus() != 0x10){
        return 1;
    }
    //select devise and send read bit
    twi_write(0x3D);
    if (twi_getStatus() != 0x40){
        return 1;
    }
    data = twi_readNACK();
    if (twi_getStatus() != 0x58){
        return 1;
    }
    twi_stop();
    return data;
}