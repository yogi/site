#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PA7
const int delay = 1000;

int main(void) {
    DDRA |= (1 << LED_PIN); // sets the data direction of pin 7 in port A to output
    
    while(1) {
        PORTA |= (1 << LED_PIN);  // writes a high value to pin 7
        _delay_ms(delay);
        
        PORTA &= ~(1 << LED_PIN); // write a low value to pin 7
        _delay_ms(delay);
    }

    return 0;
}