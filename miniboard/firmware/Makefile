MAKE:= make

# UART Bootloader flashing port
export FLASHTTY:=/dev/ttyUSB0

# Used to suppress printing of command
Q := @
export Q

.PHONY: all build clean

all: build

build:
	$(Q)$(MAKE) --quiet -C src/

clean:
	$(Q)$(MAKE) --quiet -C src/ clean