# Miniboard
This folder contains the code for the onboard Rover miniboard.

# On-Chip Resource Allocations
## UARTS
- UART0: Data radio - comm.h (and USB serial port)
- UART1: Saberteeth/AX12 - sabertooth.h/ax12.h (switch between using analog switch)
- UART2: GPS (38400 baud) - gps.h
- UART3: Camera Control/S-BUS - -/sbus.h (Camera control is TX, S-BUS is  RX.)

## Sabertooth/AX12 Switch control lines
The AX12 line is always connected to RXD1. The UART receiver should be enabled
or disabled as necessary. TXD1 is switched between the AX12 and sabertooth lines
by an analog switch. When PC6 is low, the sabertooth line is selected. When it is
high, the AX12 line is selected.

TODO: Add enable/disable functionality to uart module.
TODO: Add switch control line function.

## Compass
The GPS module includes a magnetometer to determine the compass heading. It is accessed over I2C (PD1/PD0).
Since the GPS module is located outside of the e-box on a somewhat long cable, a slow clock speed (perhaps 50kHz)
should be used with the I2C bus.

## IMU
The miniboard's onboard IMU is connected to the SPI peripheral. Accelerometer CS is PL3. 
Magnetometer CS is PL0. Gyroscope CS is PL1.

## ADC
Inputs to the ADC lines are as follows:

| ADC Line | Function |
| -------  | -------- |
| ADC0     | Battery voltage divider |
| ADC1     | Potentiometer 3 |
| ADC2     | Potentiometer 1 |
| ADC3     | Potentiometer 4 |
| ADC4     | Potentiometer 2 |
| ADC5     | Potentiometer 5 |

Potentiometers will be connected to ground and 5V, so the ratiometric
measurement mode should be used.

The battery voltage divider is a 5.1k (high side) and 1k (low side) divider
from the miniboard's power input terminal. Due to a bug in the system (it appears
to be a hardware bug), the ADC_REF_* modes cannot be used after the ADC_RATIOMETRIC
mode has been used. Instead, measure both the battery voltage divider input and the
internal 1.1V reference with the ratiometric mode, then use that to compute the voltage.

## FPV Switch
The FPV video source switch is controlled by PH6 (A), PB4 (B), PB5 (C), and PB6 (D).

| Selected Input | PH6 | PB4 | PB5 | PB6 |
|  ------------- | --- | --- | --- | --- |
| 1              |  L  |  L  |  L  |  H  |
| 2              |  H  |  L  |  L  |  H  |
| 3              |  L  |  H  |  L  |  H  |
| 4              |  H  |  H  |  L  |  H  |
| 5              |  L  |  L  |  H  |  H  |
| 6              |  H  |  L  |  H  |  H  |
| None (blank)   |  H  |  H  |  H  |  H  |

## FPV Callsign Sender
A morse-code audio stream of the callsign, to comply with FCC regs, on pin PE4. 

## GPIO
Several GPIO pins are included for future expansion.

| GPIO Line | Pin | 
| --------  | --- |
| GPIO1     | PK5 | 
| GPIO2     | PK7 |
| GPIO3     | PK6 |
| GPIO4     | PF6 |
| GPIO5     | PK3 |
| GPIO6     | PK4 | 

# Notes on Microcontroller Programming
Programming microcontrollers in C is very similar to regular C programming,
with several important differences:

1.  No Dynamically-allocated memory (malloc(), free(), realloc())
2.  Explicit variable sizes (uint8_t, int32_t, int16_t, etc. instead of int, etc)
3.  No floating point math

These aren't hard limitations; the compiler will allow you to use regular C
features if you ask it to. However, using these features is a bad idea is almost
all situations, due to the limited amount of memory and program storage available
on a microcontroller. (The device we'll be using is the Atmel atmega2560, which has
8kB of RAM and 256kB of program storage. This chip is an 8-bit microcontroller that's
part of Atmel's AVR series.)

## Peripherals and Registers
A microcontroller is a tiny CPU core along with memory, program storage, and peripherals
on a single chip. Peripherals are chunks of circuitry that interface the CPU to the outside world while providing
functions that would be difficult or impossible to do in software. For instance, the
Atmega2560 contains an analog-to-digital converter peripheral, which allows the digital CPU to
measure analog voltages.

In a microcontroller, peripherals are controlled through registers. Usually, a register
will either contain a value (such as the current count of a counter) or a collection
of individual bits. Each bit will have a specific name and function, such as turning on
a peripheral. The registers and bits are described in the microcontroller's data sheet.
**The datasheet is the most important source of information when programming a microcontroller.**

Although we might want to set or clear bits individually, they must be accessed as whole bytes.
To deal with this, there are several idioms for setting and clearing bits in registers.

## Setting and Clearing Bits
There are a few underlying concepts that must be understood for the set/clear idioms to make sense.
First, bitwise boolean AND, OR, and NOT.

Boolean logic works with values in two states, known as false and true (in logic), 0 and 1 (in software),
or low and high (in hardware). Operations such as AND, OR, and NOT yield an output based on their inputs. The tables
below are called "truth tables"; they show the output state of an operation in each possible combination
of inputs. (A and B are the inputs to the operation.)

### AND Operation
| A | B | AND |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

AND yields 1 if and only if *all* of its inputs are 1.

### OR Operation
| A | B | OR |
|---|---|---|
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 1 |

OR yields 1 if *any* its inputs are 1.

### NOT Operation
| A | NOT |
|---|---|
| 1 | 0 |
| 0 | 1 |

NOT yields the opposite of its input. (It is also known as inversion.)

### Bit Positions
Bits are numbered starting from zero from least significant to most significant.
(The least significant bit is often called the LSB, and the most signicant the MSB.
However, these terms can also mean the least and most significant *bytes*.)
For instance, the following byte is equal to 0xA6 in hexadecimal, or 166 decimal:

| Bit Number | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
| ------     | --- | --- | --- | --- | --- | --- | --- | --- |
| Bit Value  | 1 | 0 | 1 | 0 | 0 | 1 | 1 | 0 |

### Using OR to Set Bits

In C, *bitwise* boolean operations operate on individual bit positions. 
(They are distinct from logical boolean operations, which operate on the true/false
state of an entire expression.) Here are a few examples:

```
   0110      0110       0110      0110
OR 1000   OR 0100    OR 1111   OR 0000
   ----      ----       ----      ----
   1110      0110       1111      0110
```
Notice that the output has a 1 wherever either input has a 1. Also, notice that a value with all 0s except for a
given bit causes the output to be a copy of the other input, with that bit position set in the output. Thus,
the bitwise OR operator can used to set individual bits. In C, the bitwise OR operator is represented with a
single pipe character (`|`).

### Using AND to Clear Bits
In a complementary way, the bitwise AND operator can be used to clear individual bits. Examples:

```
    0110      0110       0110      0110
AND 1101  AND 1010   AND 1111  AND 0000
    ----      ----       ----      ----
    0100      0010       0110      0000
```
Notice that a value with all 1s except for a single zero clears the bit in that position. In C, the
bitwise AND operator is represented with a single ampersand (`&`).

### Generating 0s and 1s in Given Positions
Now that we know how to set and clear bits with 1s and 0s, we need a way to easily generate these values.
Luckily, the left shift operator (<<) does this. The left shift operator takes a value on its left and
a number of bit positions to shit on its right. For our purposes, the value to be shifted will always be 1. 

Examples (the outputs will be presented padded to four places, to make things clearer):

```
1 << 0 = 0001 
1 << 1 = 0010
1 << 2 = 0100
1 << 3 = 1000
```
As you can see, the idiom `(1 << n)` evaluates to a value of all zeros, except for a one in bit position n.
Using this idiom and the OR operator, we can easily set individual bits:
```
/* Since we're now working with C code, the suffix b will be used to designate a binary value. */
1010b | (1 << 2) == 1110b
```

### Using Generated Values to Set Bits
What if we want to set a bit in a register? This is commonly needed to turn on peripherals or
enable options. Example:

```
/* This is a real example from the ADC (analog to digital converter) module.
 * Here the Start Conversion bit is set in ADC control register A, 
 * directing the ADC to measure the voltage.
 * Both ADCSRA and ADSC hace already been defined by the AVR-libc header files. */
ADCSRA = ADCSRA |  (1 << ADSC);
```

The code is a bit clunky. We need to specify the register name twice, and (1 << n) is a lot to type.
Luckily, there are ways around both these issues:

```
/* To avoid typing the register name twice, we can use a C assignment operator.
 * An assignment operator combines assignment (=) with an operation (+, -, |, etc.).
 * For bitwise OR, the assignment operator is |=. All assignment operators are the operator
 * followed by an equals sign, with no space between them. The code below is equivalent to the previous
 * sample: */
ADCSRA |= (1 << ADSC);

/* The (1 << n) notation is still a bit clunky. AVR-libc provides a macro to help: _BV().
 * The BV stands for bit value. Like (1 << n), it converts a bit number to value with the
 * given bit set. As before the code below is equivalent to the previous code, and this is
 * the code that is actually used in the ADC module: */
ADCSRA |= _BV(ADSC);
```

### Using Generated Values to Clear Bits
To clear bits, we need a value with all ones except for a single zero. We can get such a value
by inverting the output of `(1 << n)` (or `_BV(n)`) using the NOT operator. In C, the NOT operator
is represented by the tilde (`~`). It works on the value to its right:

```
~1101b == ~(1 << 1) == ~_BV(1)
```

Combining what we've learned, we can clear bits in registers:

```
/* This sort of code turns off a GPIO pin. It's often used when blinking an LED.
 * Each line is equivalent to the one before it. */
PORTD = PORTD & ~(1 << PD4);
PORTD &= ~(1 << PD4);
PORTD &= ~_BV(PD4);