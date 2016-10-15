MCU:= atmega2560
PROGRAMMER:= stk500
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

# Source filename, without extension
FILE:=base


.PHONY: clean fuses download

# By default, compile and download.
all: $(FILE).bin download

# Burn fuses
fuses:
	@$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m -U efuse:w:$(EFUSE):m

# Compile & link
$(FILE).bin: $(FILE).c
	@$(CC) -mmcu=$(MCU) $(CFLAGS) -I. $(FILE).c -o $(FILE).elf $(LIBS)
	@$(OBJCOPY) -O binary -R .eeprom $(FILE).elf $(FILE).bin

# Download firmware, without changing fuses or eeprom.
download:
	@$(AVRDUDE) -p $(MCU) -c $(PROGRAMMER) -P $(PROGRAMMER_PORT) -U flash:w:$(FILE).bin:r

#Delete generated files
clean:
	@rm -f *.hex *.elf *.bin