/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * adc.c - ADC Module.
 */
#ifndef ADC_H
#define ADC_H
#include <stdint.h>

/* ADC Reference Options */
#define ADC_REF_RATIOMETRIC 1 /* ADC Reference is 5V AVcc voltage. */
#define ADC_REF_2V56 3 /* ADC Reference is 2.56V internal reference. */
#define ADC_REF_1V1 2 /* ADC Reference is 1.1V internal reference. */
#define ADC_CLK_PRESCALE 5 /* ADC clock prescaler (change if you need faster response) */

/* Return the voltage (in mV) of the given ADC channel using the given reference.
 * NOTE: Aim for highest accuracy possible. 100us is an acceptable conversion time.
 * If a single 10bit conversion takes much less than this time, incorporate averaging
 * and oversampling to increase the accuracy. */
uint16_t adc_voltage(uint8_t channel, uint8_t reference);
#endif
