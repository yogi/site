/*
  Multple Serial test

 Receives from the main serial port, sends to the others.
 Receives from serial port 1, sends to the main serial (Serial 0).

 This example works only with boards with more than one serial like Arduino Mega, Due, Zero etc

 The circuit:
 * Any serial device attached to Serial port 1
 * Serial monitor open on Serial port 0:

 created 30 Dec. 2008
 modified 20 May 2012
 by Tom Igoe & Jed Roach
 modified 27 Nov 2015
 by Arturo Guadalupi

 This example code is in the public domain.

*/

void setup() {
  Serial.begin(9600);
}

static int i = 48;
static int sleep = 1000;

void loop() {
  Serial.write(i++);
  delay(sleep);
  if (i >= 58) {
    i = 48;
  }
}
