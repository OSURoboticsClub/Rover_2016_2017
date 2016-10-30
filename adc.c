
#include "adc.h"
#include <avr/io.h>

uint16_t adc_voltage(uint8_t channel, uint8_t reference)
{
	ADCSRB = 0x00;
	//adc channels ADC8-ADC15 map to b100000-b100111
	//MUX5 is in the ADCSRB instead of ADMUX register
	if(channel > 7)
		ADCSRB |= _BV(MUX5);
	else
		ADCSRB &= ~_BV(MUX5);


	//select the ADC channel and reference 
	ADMUX = (0x3F & channel) | (reference << 8);
	//select ADC clock prescaler
	ADCSRA = _BV(ADEN) | (ADC_CLK_PRESCALE & 0x7);
	
	//disable digital input on this channel
	if(channel < 8)
		DIDR0 |= _BV(channel); 
	else
		DIDR2 |= _BV(channel & 0x07);
	
	//start ADC conversion
	ADCSRA |= _BV(ADSC);
	
	//wait for conversion to complete
	while(ADCSRA & _BV(ADSC));

	//get ADC values from memory
	uint8_t lsb = ADCL;
	uint8_t msb = ADCH;
	uint16_t adc = msb << 8 | lsb;

	//floating point math is intentionally avoided because it's very expensive on AVR
	//convert from 10-bit ADC value to millivolts
	switch(reference)
	{
		//using weird constants because multiplication overflow
		case ADC_REF_RATIOMETRIC: return adc * 50 / 10; //approximately adc * 5000 / 1024
		case ADC_REF_2V56:        return adc * 25 / 10;
		case ADC_REF_1V1:         return adc * 11 / 10;
		default: return adc;
	}
}



