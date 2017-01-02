/* OSU Robotics Club Rover 2016
 * Miniboard Firmware
 *
 * callsign.c - Video stream morse code callsign sender.
 * Author(s) Aaron Cohen
 */

#include <ctype.h>
#include <string.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "callsign.h"

/* Set to true by the callsign sender when a callsign has been entered. */
bool callsign_set = false;

/* Character buffer containing the set callsign characters */
uint8_t callsign[16] = {0};

/* Buffer containing the length of each morse mark in a callsign character */
uint8_t morse_marks[6] = {0};

/* Index of the current callsign character */
uint8_t callsign_index = 0;

/* Indicates whether the current morse state is on or off */
bool signal_on = false;

/* The number of dit lengths left in the current morse state as described by
 * `signal_on` */
uint8_t dit_lengths_left = 0;

/* The index of the current morse mark within the current callsign character */
uint8_t morse_index = 0;

/* Counter for the number of times TIMER1 has called an interrupt, used to
 * subdivide TIMER1 into `DIT_LENGTH`s */
uint8_t interrupt_count = 0;

const uint8_t morse_data[168] PROGMEM = {
	1, 3, 0,       /* A */
	3, 1, 1, 1, 0, /* B */
	3, 1, 3, 1, 0, /* C */
	3, 1, 1, 0,    /* D */
	1, 0,          /* E */
	1, 1, 3, 1, 0, /* F */
	3, 3, 1, 0,    /* G */
	1, 1, 1, 1, 0, /* H */
	1, 1, 0,       /* I */
	1, 3, 3, 3, 0, /* J */
	3, 1, 3, 0,    /* K */
	1, 3, 1, 1, 0, /* L */
	3, 3, 0,       /* M */
	3, 1, 0,       /* N */
	3, 3, 3, 0,    /* O */
	1, 3, 3, 1, 0, /* P */
	3, 3, 1, 3, 0, /* Q */
	1, 3, 1, 0,    /* R */
	1, 1, 1, 0,    /* S */
	3, 0,          /* T */
	1, 1, 3, 0,    /* U */
	1, 1, 1, 3, 0, /* V */
	0, 3, 3, 0,    /* W */
	3, 1, 1, 3, 0, /* X */
	3, 1, 3, 3, 0, /* Y */
	4, 3, 1, 1, 0, /* Z */

	3, 3, 3, 3, 3, 0, /* 0 */
	1, 3, 3, 3, 3, 0, /* 1 */
	1, 1, 3, 3, 3, 0, /* 2 */
	1, 1, 1, 3, 3, 0, /* 3 */
	1, 1, 1, 1, 3, 0, /* 4 */
	1, 1, 1, 1, 1, 0, /* 5 */
	3, 1, 1, 1, 1, 0, /* 6 */
	3, 3, 1, 1, 1, 0, /* 7 */
	3, 3, 3, 1, 1, 0, /* 8 */
	3, 3, 3, 3, 1, 0  /* 9 */
};

const uint8_t morse_lookup[36] PROGMEM = {
	0,   /* A */
	3,   /* B */
	8,   /* C */
	13,  /* D */
	17,  /* E */
	19,  /* F */
	24,  /* G */
	28,  /* H */
	33,  /* I */
	36,  /* J */
	41,  /* K */
	45,  /* L */
	50,  /* M */
	53,  /* N */
	56,  /* O */
	60,  /* P */
	65,  /* Q */
	70,  /* R */
	74,  /* S */
	78,  /* T */
	80,  /* U */
	84,  /* V */
	89,  /* W */
	93,  /* X */
	98,  /* Y */
	103, /* Z */

	108, /* 0 */
	114, /* 1 */
	120, /* 2 */
	126, /* 3 */
	132, /* 4 */
	138, /* 5 */
	144, /* 6 */
	150, /* 7 */
	156, /* 8 */
	162  /* 9 */
};

void load_morse(uint8_t ch) {
	uint8_t lookup_index;
	uint8_t lookup_offset;
	uint8_t mark_index;
	uint8_t const *morse_ch;

	if (isalpha(ch)) {
		lookup_index = toupper(ch) - 'A';
	} else if (isdigit(ch)) {
		lookup_index = ch - '0' + 26;
	} else {
		return;
	}

	// Clear the morse marks buffer
	memset(morse_marks, 0, 6);

	// Perform the character lookup and load the morse marks buffer
	lookup_offset = pgm_read_byte(&morse_lookup[lookup_index]);
	morse_ch = &morse_data[lookup_offset];
	mark_index = 0;

	while (pgm_read_byte(&morse_ch[mark_index]) != 0)
	{
		morse_marks[mark_index] = pgm_read_byte(&morse_ch[mark_index]);
		mark_index += 1;
	}

	morse_marks[mark_index] = 0;
}

ISR(TIMER1_OVF_vect)
{
	if (!callsign_set)
		return;

	// Start morse state transition
	if (dit_lengths_left == 0)
	{
		/* Transition from an "on" morse state to an "off" one
		 * (Empty space between marks, callsign characters, or words) */
		if (signal_on)
		{
			// Toggle the morse state and go to the next morse mark
			signal_on = false;
			morse_index += 1;

			// Reached end of callsign character or word
			if (morse_marks[morse_index] == 0)
			{
				// Go to the next callsign character and reset the morse index
				callsign_index += 1;
				morse_index = 0;

				/* If the end of the callsign has been reached, go back to the
				 * beginning after a seven dit space */
				if (callsign[callsign_index] == '\0')
				{
					callsign_index = 0;
					dit_lengths_left = 7;
				}
				/* If a space has been reached, increment the callsign index
				 * again and set the empty space to seven dits */
				else if (callsign[callsign_index] == ' ')
				{
					callsign_index += 1;
					dit_lengths_left = 7;
				}
				/* If we're just going to the next letter, set the empty space
				 * to 3 dits */
				else
				{
					dit_lengths_left = 3;
				}

				// Load the next callsign character into morse
				load_morse(callsign[callsign_index]);
			}
			else // Single dit space between morse marks
			{
				dit_lengths_left = 1;
			}
		}
		/* Transition state to begin the next morse mark or callsign
		 * character */
		else
		{
			signal_on = true;
			dit_lengths_left = morse_marks[morse_index];
		}
	}

	if (interrupt_count++ == DIT_LENGTH)
	{
		dit_lengths_left -= 1;
		interrupt_count = 0;
	}

	if (signal_on)
		CALLSIGN_BANK ^= _BV(CALLSIGN_PIN);
	else
		CALLSIGN_BANK &= ~(CALLSIGN_BANK & _BV(CALLSIGN_PIN));
}

void set_callsign(uint8_t *callsign_str, uint8_t callsign_len){

	// Set the callsign and the first morse character
	callsign_set = true;
  	strncpy(callsign, callsign_str, callsign_len);
	load_morse(callsign[0]);

	// Enable output pin 
	CALLSIGN_DDR |= _BV(CALLSIGN_PIN);

	// No TIMER1 prescaling
	TCCR1B |= _BV(CS10);

	// Enable interrupt on overflow for TIMER1
	TIMSK1 |= _BV(TOIE1);

	// Clear TIMER1 counter
	TCNT1 = 0;
}
