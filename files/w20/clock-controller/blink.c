// Yogi Kulkarni
// 
// Includes code from:
//
// Neil Gershenfeld
// 10/25/12
//
// (c) Massachusetts Institute of Technology 2012
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all
// liability.
//

#include <avr/io.h>
#include <util/delay.h>
#include "usi_i2c_master.c"

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 102 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay

#define digit_bus_port PORTA
#define digit_bus_direction DDRA
#define digit_bus_pin_out (1 << PA0)

#define serial_port PORTB
#define serial_direction DDRB
#define serial_pin_out (1 << PB2)

#define trigger_port PORTA
#define trigger_direction DDRA
#define trigger_pin_out (1 << PA1)

#define echo_port PORTA
#define echo_direction DDRA
#define echo_pin_in (1 << PA2)
#define echo_pins PINA

#define TURN_OFF_DISPLAY 11
#define DARK_THRESHOLD 155

void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
    //
    // send character in txchar on port pin
    //    assumes line driver (inverts bits)
    //
    // start bit
    //
    clear(*port,pin);
    bit_delay();
    //
    // unrolled loop to write data bits
    //
    if bit_test(txchar,0)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,1)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,2)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,3)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,4)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,5)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,6)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,7)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    //
    // stop bit
    //
    set(*port,pin);
    bit_delay();
    //
    // char delay
    //
    bit_delay();
}

// Convert normal decimal numbers to binary coded decimal
char decToBcd(char val) {
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
char bcdToDec(char val) {
  return( (val/16*10) + (val%16) );
}

#define RTC_I2C_ADDRESS 0x68

void set_time(char second, char minute, char hour, char dayOfWeek, char dayOfMonth, char month, char year) {
    // sets time and date data to DS3231
    char data_len = 9;
    char data[data_len]; 
        
    data[0] = (RTC_I2C_ADDRESS << 1);
    data[1] = 0;                        // set next input to start at the seconds register
    data[2] = decToBcd(second);         // set seconds
    data[3] = decToBcd(minute);         // set minutes
    data[4] = decToBcd(hour);           // set hours
    data[5] = decToBcd(dayOfWeek);      // set day of week (1=Sunday, 7=Saturday)
    data[6] = decToBcd(dayOfMonth);     // set date (1 to 31)
    data[7] = decToBcd(month);          // set month
    data[8] = decToBcd(year);           // set year (0 to 99)

    USI_I2C_Master_Start_Transmission(data, data_len);
}

void print_newline() {
    put_char(&serial_port, serial_pin_out, '\n');
    char_delay();
}

void print_binary(char num) {
    int i;
    for (i = 7; i>= 0; i--) {
        put_char(&serial_port, serial_pin_out, (num & (1 << i)) ? '1' : '0');
        char_delay();
    }
    put_char(&serial_port, serial_pin_out, '|');
    char_delay();
}

void print_binary16(uint8_t num) {
    int i;
    for (i = 15; i>= 0; i--) {
        put_char(&serial_port, serial_pin_out, (num & (1 << i)) ? '1' : '0');
        char_delay();
    }
    put_char(&serial_port, serial_pin_out, '|');
    char_delay();
}

void print_dec(uint8_t num) {
    put_char(&serial_port, serial_pin_out, 48 + (num / 10000));
    char_delay();
    num = num % 10000;

    put_char(&serial_port, serial_pin_out, 48 + (num / 1000));
    char_delay();
    num = num % 1000;

    put_char(&serial_port, serial_pin_out, 48 + (num / 100));
    char_delay();
    num = num % 100;

    put_char(&serial_port, serial_pin_out, 48 + (num / 10));
    char_delay();
    num = num % 10;

    put_char(&serial_port, serial_pin_out, 48 + num);
    char_delay();

    put_char(&serial_port, serial_pin_out, '|');
    char_delay();
}

void print_num(char num) {
    put_char(&serial_port, serial_pin_out, num);
    char_delay();
    put_char(&serial_port, serial_pin_out, " ");
    char_delay();
}

void print_digits(char second, char minute, char hour, char dayOfWeek, char dayOfMonth, char month, char year) {
    print_num(second);
    print_num(minute);
    print_num(hour);
    print_num(dayOfWeek);
    print_num(dayOfMonth);
    print_num(month);
    print_num(year);
    print_newline();
}

void get_time(char *hour_tens, char *hour_units, char *minute_tens, char *minute_units) {
    {   // using explicit scopes to ensure the correct arrays get used
     
        // position the internal address for subsequent reads 
        char data_len = 2;
        char data[data_len]; 
            
        data[0] = ((RTC_I2C_ADDRESS << 1) | 0);
        data[1] = 0;                        // set DS3231 register pointer to 00h
        USI_I2C_Master_Start_Transmission(data, data_len);
    }
    {
        // read 7 bytes of time values from current position
        char time_len = 8;
        char time[time_len]; 
    
        time[0] = ((RTC_I2C_ADDRESS << 1) | 1);
        USI_I2C_Master_Start_Transmission(time, time_len);
    
        char minute, hour;
        char second, dayOfWeek, dayOfMonth, month, year;
        
        second = bcdToDec(time[1] & 0x7f);
        minute = bcdToDec(time[2]);
        hour = bcdToDec(time[3] & 0x3f);
        dayOfWeek = bcdToDec(time[4]);
        dayOfMonth = bcdToDec(time[5]);
        month = bcdToDec(time[6]);
        year = bcdToDec(time[7]);
        
        //print_digits(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
        
//        *hour_tens = hour / 10;
//        *hour_units = hour % 10;
//        *minute_tens = minute / 10;
//        *minute_units = minute % 10;

        // hardcoding time for now, till I can debug the RTC
        *hour_tens = 1;
        *hour_units = 2;
        *minute_tens = 5;
        *minute_units = 4;
    }
}

void send_time(char hour_tens, char hour_units, char minute_tens, char minute_units) {
    // send framing
    put_char(&digit_bus_port, digit_bus_pin_out, 6);
    char_delay();
    
    put_char(&digit_bus_port, digit_bus_pin_out, 5);
    char_delay();
    
    put_char(&digit_bus_port, digit_bus_pin_out, 4);
    char_delay();
    
    put_char(&digit_bus_port, digit_bus_pin_out, 3);
    char_delay();
    
    // send time
    put_char(&digit_bus_port, digit_bus_pin_out, hour_tens);
    char_delay();
    
    put_char(&digit_bus_port, digit_bus_pin_out, hour_units);
    char_delay();
    
    put_char(&digit_bus_port, digit_bus_pin_out, minute_tens);
    char_delay();
    
    put_char(&digit_bus_port, digit_bus_pin_out, minute_units);
    char_delay();
}

long handwave_distance() {
    //
    // trigger the ultrasonic sensor
    //
    
    // start with LOW to ensure a clean signal for 4 µs
    clear(trigger_port, trigger_pin_out);
    _delay_us(4);
    
    // set HIGH for 10µs 
    set(trigger_port, trigger_pin_out);
    _delay_us(10);
    
    // back LOW to end trigger
    clear(trigger_port, trigger_pin_out);

    //
    // listen for echo
    //
    
    // wait till echo signal is taken high
    while (!pin_test(echo_pins, echo_pin_in)) {
        ; // noop
    }
    
    // start timer 
    TCNT1 = 0;
    
    // loop till signal is taken LOW
    while (pin_test(echo_pins, echo_pin_in)) {
        ; // noop
    }
    
    long duration_us = TCNT1 * 8;
    
    // The speed of sound is 340 m/s or 29 microseconds per centimeter.
    // The ping travels out and back, so to find the distance of the
    // object we take half of the distance travelled.
    long cm = duration_us / 29 / 2; 
    
    return cm;
}

int main(void) {
    //
    // set clock divider to /1
    //
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
    //
    // initialize output pins
    //
    output(digit_bus_direction, digit_bus_pin_out);
    output(serial_direction, serial_pin_out);
    output(trigger_direction, trigger_pin_out);
    input(echo_direction, echo_pin_in);
        
    set_time(56,    // sec
             48,    // min
             1,     // hour
             4,     // day of week
             22,    // day
             4,     // month
             16);   // year 
    
    char hour_tens, hour_units, minute_tens, minute_units;
    
    //
    // init A/D
    //
    ADMUX = /*(0 << REFS2) | */ (0 << REFS1) | (0 << REFS0) // Vcc ref
      | (0 << ADLAR) // right adjust
      | (0 << MUX5) | (0 << MUX4) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // ADC7
    ADCSRA = (1 << ADEN) // enable
      | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler /128

    // init timer for ultrasonic pings
    TCCR1B |= (1 << CS11) | (1 << CS10);        /* Use 1/64 prescaler. Timer clock speed: 8 MHz / 64, each tick is 8 microseconds ~= 125 per ms  */  
    
    //
    // main loop
    //
    while (1) {
        //
        // read light sensor
        //
        // initiate conversion
        //
        print_num('S');
        
        ADCSRA |= (1 << ADSC);
        //
        // wait for completion
        //
        while (ADCSRA & (1 << ADSC)) {
            ;
        }
            
        //
        // save result
        //
        uint8_t lowADC = ADCL;
        uint8_t highADC = ADCH;
        
        print_binary(highADC);
        print_binary(lowADC);
        
        uint16_t res = lowADC;
        
        long distance_cm;
        
        if (res > DARK_THRESHOLD) {  // dark enough to turn off display
            distance_cm = handwave_distance();
            
            if (distance_cm < 30) {
                get_time(&hour_tens, &hour_units, &minute_tens, &minute_units);
                send_time(hour_tens, hour_units, minute_tens, minute_units);
            } else {
                send_time(TURN_OFF_DISPLAY, TURN_OFF_DISPLAY, TURN_OFF_DISPLAY, TURN_OFF_DISPLAY);
                print_num('D');
            }
        } else {
            get_time(&hour_tens, &hour_units, &minute_tens, &minute_units);
            send_time(hour_tens, hour_units, minute_tens, minute_units);
        }
        _delay_ms(1000);
    }
}