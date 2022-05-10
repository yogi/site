PROJECT    = 7segment
DEVICE     = attiny45
CLOCK      = 1000000
PROGRAMMER = -c usbtiny
OBJECTS    = $(PROJECT).o

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	$(PROJECT).hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:$(PROJECT).hex:i

# Xcode uses the Makefile targets "", "clean" and "install"
install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID $(PROJECT).hex

clean:
	rm -f $(PROJECT).hex $(PROJECT).elf $(OBJECTS)

# file targets:
$(PROJECT).elf: $(OBJECTS)
	$(COMPILE) -o $(PROJECT).elf $(OBJECTS)

$(PROJECT).hex: $(PROJECT).elf
	rm -f $(PROJECT).hex
	avr-objcopy -j .text -j .data -O ihex $(PROJECT).elf $(PROJECT).hex
	avr-size --format=avr --mcu=$(DEVICE) $(PROJECT).elf

# Targets for code debugging and analysis:
disasm:	$(PROJECT).elf
	avr-objdump -d $(PROJECT).elf

cpp:
	$(COMPILE) -E $(PROJECT).c
