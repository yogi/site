---
layout: page
title: Week 10 & 11 - Mechanical & Machine Design 
permalink: w10-machine.html
---

Class Notes: [mechanical](http://academy.cba.mit.edu/classes/mechanical_design/index.html), [machine](http://academy.cba.mit.edu/classes/machine_design/index.html)

## Assignment

> (group assignment)
> 
>* make a machine, including the end effector
>* build the passive parts and operate it manually
>* automate your machine
>* document the group project and your individual contribution   

   
&nbsp;   
This was a 2 week assignment to build a machine in a group. I was in Team-1 along with:

* [Muhammed Jaseel P.](../../students/390/index.html)
* [Nisha Elsa Johnson](../../students/376/index.html)
* [Paul Anand](../../students/465/index.html)
* [Rashid Thattayil](../../students/394/index.html)
* [Sibu Saman](../../students/281/index.html)
* [Vinod Kumar B. G.](../../students/403/index.html)

We built a 2-axis multi-fab machine (plotter + lathe).

<video controls>
  <source src="../../machine_building/team_1/files/machine.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

&nbsp;

## Highlights 

* Started building a lathe, moved to building a 2-axis machine with changeable tool-heads (plotter + lathe-tool-bit in the first iteration)
* Salvaged inkjet printer parts from local scrapyard
* Some parts are 3D printed  
* Custom-built acrylic stages
* RAMPS controller running a g-code interpreter available over serial interface

Our machine-building journey is described in detail here: __[Team 1: 2-Axis Multi-Fab Machine](../../machine_building/team_1/index.html).__

## My Contribution

I was the "software guy" on the team and built the control system for the machine.

We did not have Gestalt or Fabnet boards in the lab, so we had to come up with an alternative. 

Sibu had a Arduino Mega + RAMPS board ([RepRap Arduino Mega Pololu Shield](http://reprap.org/wiki/Arduino_Mega_Pololu_Shield)) + 
[A4988 stepper driver](https://www.pololu.com/product/1182), which he generously loaned for the project.   


### RAMPS + Motor Test

Next step was to check if RAMPS was working fine and could move a single stepper motor.

We hooked up the boards and motor and used [RAMPS test code](http://reprap.org/wiki/RAMPS_1.4#RAMPS_1.4_test_code) to test it (I used 
Arduino IDE to compile and upload the code).

<video controls>
  <source src="../../machine_building/team_1/files/ramps-stepper-test.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

&nbsp;

It was good to see that working!


### Research

I then started looking around for approaches to drive the machine and provide a convenient interface.  

The overall approach I settled on was to have a g-code interpreter on the board and talk to it through its serial interface. 

The host machine would talk to the board using some serial-capable software like screen / pyserial / etc. 
  
The G-code itself would be written to a file or piped to a command, which would interact with the board.

Some of the options considered were:

- [Mods](http://mods.cba.mit.edu)

This is a very convenient way to provide a UI, with composable steps that can be used to load an image, transform it, 
 generate a toolpath, generate g-code and send it to the machine over serial. 

- [Makelangelo](https://www.marginallyclever.com/product/makelangelo-software/)

This is a wonderful wall-hanging mural-drawing robot, which was inspired by [Hektor](http://juerglehni.com/works/hektor), the 
spray-painting robot. 

Makelangelo provides a nice UI written in Java, which can be used to load images, transform them and print them on the robot over a serial connection. 

The author of Makelangelo has open-sourced all the code and firmware. I also found an article written by the author on 
[How to build an 2-axis Arduino CNC Gcode Interpreter](https://www.marginallyclever.com/2013/08/how-to-build-an-2-axis-arduino-cnc-gcode-interpreter/). 
This was exactly what we needed!
 
### G-code interpreter

We took the g-code interpreter from the article ([github repo](https://github.com/MarginallyClever/GcodeCNCDemo)) and 
noticed that it also had support for RAMPS.

It supported line (G00, G01) and arc (G02, G03) drawing G-code commands. 

The full source is included here:
                                                               
* [GcodeCNCDemo2Axis.ino](files/GcodeCNCDemo2Axis/GcodeCNCDemo2Axis.ino)
* [config.h](files/GcodeCNCDemo2Axis/config.h)
* [RAMPS.ino](files/GcodeCNCDemo2Axis/RAMPS.ino)

Here is a code-snippet showing the main g-code interpreter function:

<pre>
<code class="language-clike">
/**
 * Read the input buffer and find any recognized commands.  One G or M command per line.
 */
void processCommand() {
  int cmd = parsenumber('G',-1);
  switch(cmd) {
  case  0:
  case  1: { // line
    feedrate(parsenumber('F',fr));
    line( parsenumber('X',(mode_abs?px:0)) + (mode_abs?0:px),
          parsenumber('Y',(mode_abs?py:0)) + (mode_abs?0:py) );
    break;
    }
  case 2:
  case 3: {  // arc
      feedrate(parsenumber('F',fr));
      arc(parsenumber('I',(mode_abs?px:0)) + (mode_abs?0:px),
          parsenumber('J',(mode_abs?py:0)) + (mode_abs?0:py),
          parsenumber('X',(mode_abs?px:0)) + (mode_abs?0:px),
          parsenumber('Y',(mode_abs?py:0)) + (mode_abs?0:py),
          (cmd==2) ? -1 : 1);
      break;
    }
  case  4:  pause(parsenumber('P',0)*1000);  break;  // dwell
  case 90:  mode_abs=1;  break;  // absolute mode
  case 91:  mode_abs=0;  break;  // relative mode
  case 92:  // set logical position
    position( parsenumber('X',0),
              parsenumber('Y',0) );
    break;
  default:  break;
  }

  cmd = parsenumber('M',-1);
  switch(cmd) {
  case 18:  // disable motors
    disable();
    break;
  case 100:  help();  break;
  case 114:  where();  break;
  default:  break;
  }
}
</code>

</pre>


Here is the RAMPS module:

<pre>
<code class="language-clike">

//------------------------------------------------------------------------------
// CONSTANTS
//------------------------------------------------------------------------------
#define M1_STEP 54
#define M1_DIR  55
#define M1_ENA  38

#define M2_STEP 60
#define M2_DIR  61
#define M2_ENA  56

// limit switches
#define SWITCH1 3
#define SWITCH2 14

//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------
  
//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

void m1step(int dir) {
  debug("X-step ", dir);
  digitalWrite(M1_ENA,LOW);
  digitalWrite(M1_DIR, dir == 1 ? HIGH : LOW);
  digitalWrite(M1_STEP,HIGH);
  digitalWrite(M1_STEP,LOW);
}

void m2step(int dir) {
  debug("Y-step ", dir);
  digitalWrite(M2_ENA,LOW);
  digitalWrite(M2_DIR, dir == 1 ? HIGH : LOW);
  digitalWrite(M2_STEP,HIGH);
  digitalWrite(M2_STEP,LOW);
}

void disable() {
  debug("!disable", 0);
  digitalWrite(M1_ENA,HIGH);
  digitalWrite(M2_ENA,HIGH);
}


void setup_controller() {
  pinMode(M1_ENA,OUTPUT);
  pinMode(M2_ENA,OUTPUT);
  pinMode(M1_STEP,OUTPUT);
  pinMode(M2_STEP,OUTPUT);
  pinMode(M1_DIR,OUTPUT);
  pinMode(M2_DIR,OUTPUT);
}

</code></pre>

### G-code Interpreter Test

Next step was to test if the firmware was working as expected and that we could talk to it over serial.
  
In the video below you can see the interaction being demoed:

* connect to the serial port using screen
* the gcode interpreter on RAMPS outputs an intro message and a help menu of supported G-code commands
* enter G-code command: "G00 X800". This rotates the shaft clockwise 90 degrees (each step is 1.8 degrees and we are using 16 microsteps per step).
* enter G-code command: "G00 X0". This rotates the shaft anti-clockwise 90 degrees.

<video controls>
  <source src="../../machine_building/team_1/files/gcode-stepper-test.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

&nbsp;

In the video, the motor is moving very slowly because of the debugging output that I'd added for each step. 


### Command-line Interface Script 

The next step was to write a simple way to input g-code commands. We used pyserial for this.

<pre>
<code class="language-clike">
import serial
import fileinput

def consumeLines():
    while True:
        l = ser.readline().strip()
        print ">>> " + l
        if l == ">": # wait for the device to be ready for the next command
            break
    
ser = serial.Serial('/dev/cu.usbmodem1411', baudrate=9600, timeout=None) # block forever on reads
ser.isOpen()

consumeLines()

for cmd in fileinput.input():
    print "sending cmd: " + cmd.strip()
    ser.write(cmd)
    consumeLines()

ser.close()
</code></pre>

Now we can invoke it like this:

<pre>
$ python serialsend.py square.gcode
</pre>

And square.gcode can contain raw gcode commands like this:

<pre>
G00 X10000
G00 Y10000
G00 X0
G00 Y0
</pre>


### Notes / Learnings / Todos

* The arc drawing command did not work as expected, it would draw a (mostly) straight line... need to debug this.

* The motors were rotating slowly initially, until the feed-rate was changed from 10,000 to max (1,000,000), which resulted in a sleep of 1µs.

* The driver was setup to run in microstep mode, with 16 microsteps/step. Removing the jumpers to turn off micro-stepping 
    did not seem to have any effect... need to investigate this further.
    
* [grbl](https://github.com/grbl/grbl) is a full-featured, production-grade g-code interpreter.

* Measure backlash, explore linear encoders.