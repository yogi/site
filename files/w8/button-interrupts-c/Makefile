DEVICE     = attiny44
CLOCK      = 1000000
PROGRAMMER = -c usbtiny #-c stk500v2 -P avrdoper
OBJECTS    = button-int.o
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0x24:m

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	button-int.hex

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
	$(AVRDUDE) -U flash:w:button-int.hex:i

clean:
	rm -f button-int.hex button-int.elf $(OBJECTS)

# file targets:
button-int.elf: $(OBJECTS)
	$(COMPILE) -o button-int.elf $(OBJECTS)

button-int.hex: button-int.elf
	rm -f button-int.hex
	avr-objcopy -j .text -j .data -O ihex button-int.elf button-int.hex
	avr-size --format=avr --mcu=$(DEVICE) button-int.elf
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

# Targets for code debugging and analysis:
disasm:	button-int.elf
	avr-objdump -d button-int.elf

cpp:
	$(COMPILE) -E button-int.c
