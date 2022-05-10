; blink.asm
;
; Blink LED 

.include "tn44def.inc"

.org 0              ; sets the programs origin

sbi DDRA, 7         ; set bit in Data Direction Register to mark the pin as output

loop:       
sbi PORTA, 7        ; turn on the LED

clr r24             ; clear register to use as part of word             
clr r25             ; clear register to use as part of word             
delay_loop1:                                                            
adiw r24, 1         ; add 1 to the word starting at r24                 
brne delay_loop1    ; loop until overflow                               

cbi PORTA, 7        ; turn off the LED

clr r24             ; clear register to use as part of word
clr r25             ; clear register to use as part of word
delay_loop2:                                               
adiw r24, 1         ; add 1 to the word starting at r24    
brne delay_loop2    ; loop until overflow                  

rjmp loop
