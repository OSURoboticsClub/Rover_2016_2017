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
on a single chip. Peripherals interface the CPU to the outside world while providing
functions that would be difficult or impossible to do in software. For instance, the
Atmega2560 contains an analog-to-digital converter, which allows the digital CPU to
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

### OR Operation
| A | NOT |
|---|---|
| 1 | 0 |
| 0 | 1 |

NOT yields the opposite of its input. (It is also known as inversion.)

In C, *bitwise* boolean operations operate on individual bit positions. 
(They are distinct from logical boolean operations, which operate on the true/false
state of an entire expression.) Here's an example: OR'ing the values 11000000 
and 01000110:

```
   11000000
OR 01000110
   --------
   11000110
```
TODO: Finish microcontroller programming notes
