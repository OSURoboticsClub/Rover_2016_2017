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
#define CONF_A 0x00
#define	CONF_B 0x01
#define	MODE 0x02
#define X_MSB 0x03
#define X_LSB 0x04
#define	Z_MSB 0x05
#define	Z_LSB 0x06
#define	Y_MSB 0x07
#define Y_LSB 0x08

uint8_t comp_addr = 0x00;
uint8_t eepromAddress = 00; // the address in the HMC6352 EEPROM from which to request the value of the I2C Slave Address

void comp_init(void) {
	uart_enable(COMP_UART, 1000000, 1, 0);
	twi_init();
	config_rega();
	config_regb();
	mode();
}

void retrieve(){
	read_x();
	read_z();
	read_y();
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

void read_x()
{
	uint8_t xh = twi_readfr(X_MSB);
	uint8_t xl = twi_readfr(X_LSB);

	Data->mag_x = xl | (xh << 8);
}

void read_z()
{
	uint8_t zh = twi_readfr(Z_MSB);
	uint8_t zl = twi_readfr(Z_LSB);

	Data->mag_z = zl | (zh << 8);
}

void read_y()
{
	uint8_t yh = twi_readfr(Y_MSB);
	uint8_t yl = twi_readfr(Y_LSB);

	Data->mag_y = yl | (yh << 8);
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