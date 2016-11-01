/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * adc.c - ADC Module.
 * Author(s): Aaron Schraner, Aaron Cohen
 */
#include "adc.h"
#include <avr/io.h>

#define ADC_CLK_PRESCALE 5 /* ADC clock prescaler (change if you need faster response) */

/* Perform an ADC conversion and return the result. */
static uint16_t do_conversion(void){
	//start ADC conversion
	ADCSRA |= _BV(ADSC);
	
	//wait for conversion to complete
	while(ADCSRA & _BV(ADSC));

	//get ADC values from memory
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t adc = msb << 8 | lsb;
	return adc;
}


/* Return the voltage (in mV) of the given ADC channel using the given reference.
 * If the ADC_REF_RATIOMETRIC option is used, the returned value will range from
 * 0-1023, ratiometric with the supply voltage. (Useful for digitizing potentiometers.) */
uint16_t adc_voltage(uint8_t channel, uint8_t reference){
	//adc channels ADC8-ADC15 map to b100000-b100111
	//MUX5 is in the ADCSRB instead of ADMUX register
	if(channel > 7){
		ADCSRB = _BV(MUX5);
	} else {
		ADCSRB = 0;
	}
	
	//select the ADC channel and reference 
	ADMUX = (0x1F & channel) | (reference << 6);
	//select ADC clock prescaler
	ADCSRA = _BV(ADEN) | (ADC_CLK_PRESCALE & 0x7);
	
	//disable digital input on this channel
	if(channel < 8){
		DIDR0 |= _BV(channel); 
	} else {
		DIDR2 |= _BV(channel & 0x07);
	}
	
	uint32_t adc = do_conversion();
	
	switch(reference){
		case ADC_REF_RATIOMETRIC: return adc;
		case ADC_REF_2V56:        return (adc * 2560) / 1024;
		case ADC_REF_1V1:         return (adc * 1100) / 1024;
		default: return adc;
	}
}



