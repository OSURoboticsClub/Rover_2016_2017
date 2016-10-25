MCU:= atmega2560
PROGRAMMER:= arduino
PROGRAMMER_PORT:= /dev/ttyACM0

CC:=avr-gcc
OBJCOPY:=avr-objcopy
AVRDUDE:=avrdude
CFLAGS:= -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wextra -Wall -Wstrict-prototypes -std=gnu99
LIBS:=


#Fuse Settings
LFUSE:=0x
HFUSE:=0x
EFUSE:=0x

# Output filename, without extension
OUTFILE:=../miniboard
# Object file names
OBJFILES:=uart.o 
# Main file
FILE:=miniboard.c

.PHONY: clean fuses download

# By default, compile and download.
all: $(OUTFILE).bin download

# Burn fuses
fuses:
	@$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

#Compile source files
%.o: %.c %.h
	@$(CC) -mmcu=$(MCU) $(CFLAGS) -I. -c $< -o $@ $(LIBS)
	
# Compile & link
$(OUTFILE).bin: $(OBJFILES)
	@$(CC) -mmcu=$(MCU) $(CFLAGS) -I. $(FILE) $(OBJFILES) -o $(OUTFILE).elf $(LIBS)
	@$(OBJCOPY) -O binary -R .eeprom $(OUTFILE).elf $(OUTFILE).bin

# Download firmware, without changing fuses or eeprom.
download:
	@$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -U flash:w:$(FILE).bin:r

#Delete generated files
clean:
	@rm -f *.hex *.elf *.bin *.o $(OUTFILE).elf $(OUTFILE).bin