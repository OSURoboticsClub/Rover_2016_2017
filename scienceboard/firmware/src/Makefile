MCU:= atmega2560
PROGRAMMER:= arduino
PROGRAMMER_PORT:= /dev/ttyACM0
PROGRAMMER_PORT_ALT:= /dev/ttyUSB0
PROGRAMMER_PORT_OSX:= /dev/tty.usbmodem1421

SIMAVR:=simavr

CC:=avr-gcc
OBJCOPY:=avr-objcopy
AVRDUDE:=avrdude
CFLAGS:= -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wextra -Wall -Wstrict-prototypes -std=gnu99
LIBS:= -lm


# Output filename, without extension
OUTFILE:=../build/scienceboard
# Object file names
# NOTE: Order is important! For the weak-symbol based UART receive handler mapping,
# uart.o must come last.
OBJFILES:=soilprobe.o comm.o
# Main file
FILE:=sciboard.c

.PHONY: clean fuses download

# By default, compile and download.
all: $(OUTFILE).bin download

# Compile source files
%.o: %.c %.h
	@$(CC) -mmcu=$(MCU) $(CFLAGS) -I. -c $< -o $@ $(LIBS)
	
# Compile & link
$(OUTFILE).bin: $(OBJFILES) sciboard.c
	@$(CC) -mmcu=$(MCU) $(CFLAGS) -I. $(FILE) $(OBJFILES) -o $(OUTFILE).elf $(LIBS)
	@$(OBJCOPY) -O binary -R .eeprom $(OUTFILE).elf $(OUTFILE).bin
	@$(OBJCOPY) -O ihex -R .eeprom $(OUTFILE).elf $(OUTFILE).hex

# Download firmware, without changing fuses or eeprom.
download:
	@if [ -c $(PROGRAMMER_PORT) ];\
	then \
		$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -D -U flash:w:$(OUTFILE).bin:r;\
	else \
		if [ -c $(PROGRAMMER_PORT_ALT) ];\
		then \
			$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT_ALT) -D -U flash:w:$(OUTFILE).bin:r; \
		else \
			if [ -c $(PROGRAMMER_PORT_OSX) ];\
			then \
				$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT_OSX) -D -U flash:w:$(OUTFILE).bin:r; \
			else \
				echo "Port $(PROGRAMMER_PORT), $(PROGRAMMER_PORT_ALT), or $(PROGRAMMER_PORT_OSX) not found; skipping programming."; \
			fi; \
		fi; \
	fi;

#Delete generated files
clean:
	@rm -f *.hex *.elf *.bin *.o $(OUTFILE).elf $(OUTFILE).bin
