#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include "avr/interrupt.h"
#include "queue.h"

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

#define TURN_OFF_DISPLAY 11  // has to match the value in clock-controller

char DIGIT_SEGMENTS[11][7] = {
     {'A', 'B', 'C', 'D', 'E', 'F'},            // 0
     {'B', 'C'},                                // 1
     {'A', 'B', 'D', 'E', 'G'},                 // 2
     {'A', 'B', 'C', 'D', 'G'},                 // 3
     {'B', 'C', 'F', 'G'},                      // 4
     {'A', 'C', 'D', 'F', 'G'},                 // 5
     {'A', 'C', 'D', 'E', 'F', 'G'},            // 6
     {'A', 'B', 'C'},                           // 7
     {'A', 'B', 'C', 'D', 'E', 'F', 'G',},      // 8
     {'A', 'B', 'C', 'F', 'G',},                // 9
     {'A', 'D', 'G',}                           // 10 - this is the pattern shown when an invalid digit is received 
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
    5, // 9
    3  // 10
};

#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_delay_time 100 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay

#define serial_pins PINB
#define SERIAL_IN_PIN (1 << PB1)

volatile char digit = 10;  // default value (displayed as 3 horizontal lines)


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

void display(int d, int duration_ms) {
    int i;
    if (d == TURN_OFF_DISPLAY) {
        return; 
    }
    if (d < 0 || d > 9) {  
        d = 10;                 // default pattern to show if an invalid digit is received (initial value is also set to 10)
    }
    char *segments = DIGIT_SEGMENTS[d];
    int num_segments = DIGIT_NUM_SEGMENTS[d];
    int cycles = duration_ms / (FLASH_DELAY_MS * num_segments);
    for (i = 0; i < cycles; i++) {
        flash_segments(segments, num_segments);
    }
}

#define DIGIT_POSITION 0 // position of this digit in displaying the time (this needs to be changed for each digit, and needs to be between 0 - 3 inclusive) 

char time[4] = { 0, 0, 0, 0 };

void update(char c) {
    queue_enqueue(c);
    
    if (!queue_isFull()) return;
 
    if (queue_dequeue() != 6) return;
    if (queue_dequeue() != 5) return;
    if (queue_dequeue() != 4) return;
    if (queue_dequeue() != 3) return;

    time[0] = queue_dequeue();
    time[1] = queue_dequeue();
    time[2] = queue_dequeue();
    time[3] = queue_dequeue();

    digit = time[DIGIT_POSITION];
}

ISR(PCINT0_vect) {          // has to be PCINT0_vect and not PCINT1_vect even though I'm enabling PCINT1
    volatile unsigned char *pins = &serial_pins;
    unsigned char pin = SERIAL_IN_PIN;      
    char tmp = 0;
    char *rxbyte = &tmp;
    
    //
    // read character into rxbyte on pins pin
    //    assumes line driver (inverts bits)
    //
    *rxbyte = 0;

    // assume we have just seen the start bit (pin going low)
    // check that it is indeed 0, otherwise return
    if pin_test(*pins, pin)
        return;
        
    //
    // delay to middle of first data bit
    //
    half_bit_delay();
    bit_delay();
    //
    // unrolled loop to read data bits
    //
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
    else
      *rxbyte |= (0 << 0);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
    else
      *rxbyte |= (0 << 1);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
    else
      *rxbyte |= (0 << 2);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
    else
      *rxbyte |= (0 << 3);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
    else
      *rxbyte |= (0 << 4);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
    else
      *rxbyte |= (0 << 5);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
    else
      *rxbyte |= (0 << 6);
    bit_delay();
    if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
    else
      *rxbyte |= (0 << 7);
    //
    // wait for stop bit
    //
    bit_delay();
    half_bit_delay();
    
    if (*rxbyte >= 48 && *rxbyte <= 57) {   // translate ASCII to digit
        *rxbyte -= 48;
    }
    
    update(*rxbyte);
}


int main(void) {
    //
    // set clock divider to /1
    //
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
    
    DDRB &= (~ (1 << PB1));         // set serial-in pin to input
    PORTB |= (1 << PB1);            // ... and enable pull-up by driving it high
    
    GIMSK |= (1 << PCIE);           // enable pin change interrupt 
    PCMSK |= (1 << PCINT1);         // ... only for serial-in pin
    
    sei();                          // enable interrupts globally
    
    while(1) {
       display(digit, 1000);
    }
    
    return 0;
}
