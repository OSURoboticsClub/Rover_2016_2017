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

/* Return the potentiometer position (from 0-255) of a
 * miniboard potentiometer channel. */
uint8_t pot_channel(uint8_t channel);

/* Return the battery voltage (in mV). */
uint16_t battery_mV(void);

/* Return the voltage (in mV) of the given ADC channel using the given reference.
 * If the ADC_REF_RATIOMETRIC option is used, the returned value will range from
 * 0-1023, ratiometric with the supply voltage. (Useful for digitizing potentiometers.) */
uint16_t adc_voltage(uint8_t channel, uint8_t reference);

#endif
