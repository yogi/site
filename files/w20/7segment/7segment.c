#include <avr/io.h>
#include <util/delay.h>
#include "avr/interrupt.h"

#define SCK_PIN PB2
#define MOSI_PIN PB0

#define ABC_ANODE   SCK_PIN
#define A           PB4
#define B           PB3
#define C           MOSI_PIN

#define DEF_ANODE   MOSI_PIN
#define D           PB4
#define E           PB3
#define F           SCK_PIN

#define G_ANODE     PB3
#define G           PB4

#define DIR_REG   DDRB

#define output(pin) (DIR_REG |= (1 << pin)) // set port direction for output
#define input(pin) (DIR_REG &= (~ (1 << pin))) // set port direction for input
#define set(pin) (PORTB |= (1 << pin)) // set port pin
#define clear(pin) (PORTB &= (~ (1 << pin))) // clear port pin

const int FLASH_DELAY_MS = 1;
const int DIGIT_DISPLAY_MS = 1000;

char DIGIT_SEGMENTS[10][7] = {
     {'A', 'B', 'C', 'D', 'E', 'F'},            // 0
     {'B', 'C'},                                // 1
     {'A', 'B', 'D', 'E', 'G'},                 // 2
     {'A', 'B', 'C', 'D', 'G'},                 // 3
     {'B', 'C', 'F', 'G'},                      // 4
     {'A', 'C', 'D', 'F', 'G'},                 // 5
     {'A', 'C', 'D', 'E', 'F', 'G'},            // 6
     {'A', 'B', 'C'},                           // 7
     {'A', 'B', 'C', 'D', 'E', 'F', 'G',},      // 8
     {'A', 'B', 'C', 'F', 'G',}                 // 9
};

int DIGIT_NUM_SEGMENTS[] = {
    6, // 0
    2, // 1
    5, // 2
    5, // 3
    4, // 4
    5, // 5
    6, // 6
    3, // 7
    7, // 8
    5  // 9
};



#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 100 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define led_delay() _delay_ms(100) // LED flash delay

#define START_BIT_CHECK_TIMEOUT 2 // 5 ms

#define serial_pins PINB
#define SERIAL_IN_PIN (1 << PB1)

volatile char digit = 1;


int anode_pin_for(char led) {
    if (led == 'A' || led == 'B' || led == 'C') 
        return ABC_ANODE;
    else if (led == 'D' || led == 'E' || led == 'F') 
        return DEF_ANODE;
    else  
        return G_ANODE;
}

int led_pin_for(char led) {
    if (led == 'A' || led == 'D' || led == 'G')
        return PB4;
    else if (led == 'B' || led == 'E')
        return PB3;
    else if (led == 'C')
        return MOSI_PIN;
    else // 'F'
        return SCK_PIN;
}

void high_impedance(pin) {
    input(pin);
    clear(pin);
}

void flash(char led) {
    int anode_pin = anode_pin_for(led);
    int led_pin = led_pin_for(led);
    
    output(anode_pin);    
    output(led_pin);
        
    set(anode_pin);
    clear(led_pin);

    _delay_ms(FLASH_DELAY_MS);
    
    high_impedance(anode_pin);    
    high_impedance(led_pin);
}

void flash_segments(char segments[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        flash(segments[i]);
    }
}

void display(int digit, int duration_ms) {
    int i;
    char *segments = DIGIT_SEGMENTS[digit];
    int num_segments = DIGIT_NUM_SEGMENTS[digit];
    int cycles = duration_ms / (FLASH_DELAY_MS * num_segments);
    for (i = 0; i < cycles; i++) {
        flash_segments(segments, num_segments);
    }
}

int main(void) {
    int i = 0;
    while(1) {
        display(i, DIGIT_DISPLAY_MS);
        _delay_ms(1000);
        if (++i >= 10)
            i = 0;
    }
    
    return 0;
}
