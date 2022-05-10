#include <avr/io.h>
#include <SoftwareSerial.h>

const int rx=10;
const int tx=13;

SoftwareSerial mySerial(rx,tx);

void setup() { 
  pinMode(rx,INPUT);
  pinMode(tx,OUTPUT);
  mySerial.begin(9600);
} 

static int sleep = 1000;
static char n = 0;

void loop() {
  // framing
  mySerial.write(6);
  mySerial.write(5);
  mySerial.write(4);
  mySerial.write(3);

  // time
  mySerial.write(n);
  mySerial.write(n);
  mySerial.write(n);
  mySerial.write(n);

  n++;
  if (n > 9) n = 0;

  delay(sleep);
} 
