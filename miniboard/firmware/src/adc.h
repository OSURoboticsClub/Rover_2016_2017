/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * adc.c - ADC Module.
 */
#include <stdint.h>

/* ADC Reference Options */
#define ADC_REF_RATIOMETRIC 0 /* ADC Reference is 5V AVcc voltage. */
#define ADC_REF_2V56 2 /* ADC Reference is 2.56V internal reference. */
#define ADC_REF_1V1 3 /* ADC Reference is 1.1V internal reference. */

/* Return the voltage (in mV) of the given ADC channel using the given reference.
 * NOTE: Aim for highest accuracy possible. 100us is an acceptable conversion time.
 * If a single 10bit conversion takes much less than this time, incorporate averaging
 * and oversampling to increase the accuracy. */
uint16_t adc_voltage(uint8_t channel, uint8_t reference);