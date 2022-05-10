#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_DDR     DDRA
#define LED         PA7
#define LED_PORT    PORTA

ISR(TIM0_OVF_vect) {
  // interrupt occurs every:
  //    1000000 / 1024 * 256 
  //    = ~ 250000 clocks 
  //    = ~250 ms (since clock is 1mhz and 1 clock cycle = 1µs)
  LED_PORT ^= (1 << LED);   //toggle led pin 
} 
 
void initTimerInterrupt(void) {
  TCCR0B |= (1<<CS02) | (1<<CS00);          //clock select: prescale timer clk/1024
  
  TIMSK0 |= (1<<TOIE0);                     //enable timer overflow interrupt      
  
  sei();                                    // globally enable interrupts
}

int main(void) {
    LED_DDR |= (1 << LED);              // mark LED as output 
    
    initTimerInterrupt();

    while(1);                           // loop forever
    
    return 0;                           // never gets here
}
