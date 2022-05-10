DEVICE     = attiny44
CLOCK      = 1000000
PROGRAMMER = -c usbtiny
OBJECTS    = blink.o
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0x24:m

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	blink.hex

flash:	all
	$(AVRDUDE) -U flash:w:blink.hex:i

clean:
	rm -f blink.hex blink.eep.hex blink.obj blink.cof $(OBJECTS)

blink.hex: 
	avra blink.asm
	
disasm:	blink.hex
	avr-objdump -j .sec1 -d -m avr blink.hex

