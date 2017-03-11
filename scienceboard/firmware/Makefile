MAKE:= make


# Used to suppress printing of command
Q := @
export Q

.PHONY: all build clean simulate

all: build

build:
	$(Q)$(MAKE) --quiet -C src/

clean:
	$(Q)$(MAKE) --quiet -C src/ clean
	
simulate:
	$(Q)$(MAKE) --quiet -C src/ simulate