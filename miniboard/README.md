# Miniboard
This folder contains the code for the onboard Rover miniboard.

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
Notice how there's a 1 in all bit positions where a 1 was present in either
