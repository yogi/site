#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define CLOCK 1e6
#define BAUD 9600

#define STX_PORT PORTA
#define STX_PIN  0

#define LED_PIN PA7

#define BUTTON_PORT PORTA
#define BUTTON      PA3
#define BUTTON_PIN  PINA

#define MICROSECONDS_PER_BIT (CLOCK / BAUD) 

static uint8_t start_sending = 0;


ISR(PCINT0_vect) {
    blinkLED();
    start_sending = 1;
}


void initPinChangeInterrupt(void) {
    GIMSK |= (1 << PCIE0);              // enable pin change interrupt 0
    PCMSK0 |= (1 << BUTTON);            // enable pin change only for button
}


void sendByte(uint8_t c) {
    STX_PORT &= ~(1<<STX_PIN);            // start bit
    _delay_us(MICROSECONDS_PER_BIT);              // bit duration
    
    uint8_t mask;
    
    for(mask = 0x01; mask; mask <<= 1) {        // data bits
        if( c & mask ) {
            STX_PORT |= 1<<STX_PIN;           // data bit 1 
        } else {
            STX_PORT &= ~(1<<STX_PIN);        // data bit 0
        }
        _delay_us(MICROSECONDS_PER_BIT);
    }
    
    STX_PORT |= 1<<STX_PIN;           // stop bit 
    _delay_us(MICROSECONDS_PER_BIT);
}


void blinkLED() {
    PORTA |= (1 << LED_PIN);  // writes a high value to pin 7
    _delay_ms(100);
    PORTA &= ~(1 << LED_PIN); // write a low value to pin 7
    _delay_ms(100);
}

int main(void) {
    DDRA |= (1 << LED_PIN);
    BUTTON_PORT |= (1 << BUTTON);       // mark button as input and enable pull-up resistor
    
    initPinChangeInterrupt();
    sei();                              // globally enable interrupts

    uint8_t i = 0;      

    for(;;){
        if(start_sending) {
            sendByte(65);  // Send byte of data
            i++;                // Add one to the counter
            
            if(i == 10){        // End of Data
                i = 0;              // Reset Counter
                _delay_ms(500);      // Delay a bit before sending another block of data
                blinkLED();
            }
        }
    }
    
    return 0;
}

