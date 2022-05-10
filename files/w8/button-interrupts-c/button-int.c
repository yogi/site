#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_DDR     DDRA
#define LED         PA7
#define LED_PORT    PORTA

#define BUTTON_PORT PORTA
#define BUTTON      PA3
#define BUTTON_PIN  PINA

const int delay = 100;

ISR(PCINT0_vect) {
    // by default the bit will have a high value because of the pull-up resistor
    if(bit_is_set(BUTTON_PIN, BUTTON)) {    // this is true when a pressed button is released
        LED_PORT |= (1 << LED);             // turn LED on
        _delay_ms(1000);                    // so we can see the change
    } else {                                // this will happen when the button is pressed
        LED_PORT &= ~(1 << LED);            // turn LED off
        _delay_ms(1000);
    }    
}

void initPinChangeInterrupt(void) {
    GIMSK |= (1 << PCIE0);              // enable pin change interrupt 0
    PCMSK0 |= (1 << BUTTON);            // enable pin change only for button
    sei();                              // enable interrupts globally
}

int main(void) {
    LED_DDR |= (1 << LED);              // mark LED as output 
    BUTTON_PORT |= (1 << BUTTON);       // mark button as input and enable pull-up resistor
    
    initPinChangeInterrupt();

    while(1) {
        _delay_ms(delay);
        LED_PORT ^= (1 << LED);         // toggle the LED
    }
    return 0;
}
