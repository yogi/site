---
layout: page
title: Week 18 - Applications & Implications
permalink: w18-applications.html
---

[Class Notes](http://academy.cba.mit.edu/classes/applications_implications/index.html)   

## Assignment

>  propose a final project that integrates the range of units covered:
> 
>  * what will it do?
>  * who's done what beforehand?
>  * what materials and components will be required?
>  * where will they come from?
>  * how much will it cost?
>  * what parts and systems will be made?
>  * what processes will be used?
>  * what tasks need to be completed?
>  * what questions need to be answered?
>  * what is the schedule?
>  * how will it be evaluated?
> 
>  projects can be separate or joint, but need to show individual mastery of all of the skills   
>  where possible, you should make rather than buy the parts of your project

## Hi! Time - Gesture Sensing Clock

As described in my [initial project proposal](w1-principles.html), I plan to build a gesture sensing clock. 

<img src="images/clock.jpg" height="300"/>

It is a clock, which:

* has an LED / LCD display
* displays the time normally in the day
* detects ambient light and turns off the display when its completely dark
* it detects gestures, maybe a waving hand, and switches on the display for a few seconds

The idea for this clock arose from a personal need. I like to sleep in pitch dark and in complete silence. Currently I have an 
 analog clock which audibly tick-tocks and even though it has flouroscent arms, is not visible to me without my specs in the dark.
 
Hi! Time would solve this problem. 

I also want make one with a distinctive, maybe even retro, look.  

## Prior Art

There are quite a few clocks which can project time onto a wall and recognize gestures. 

However, none of these turn-off the display when dark. Also most of these recognize gestures to turn on the projection 
    but the hand-motion has to be right above the clock. These can't detect gestures across the room.

Here are some similar products:

* Braun has a nicely designed [one](http://www.braun-clocks.com/clock/projection)
* There are [many](http://www.oregonscientificstore.com/c-16-projection-clocks.aspx) 
    [more](http://www.amazon.com/Best-Sellers-Home-Kitchen-Projection-Clocks/zgbs/home-garden/3734981) 
    [available](http://www.ebay.com/bhp/projection-clock)
* This is a [motion-detecting spy clock](http://www.amazon.com/TechTonic-Portable-Camera-Motion-Detection/dp/B00AY571XA) 
* [Wave Alarm](https://itunes.apple.com/app/wave-alarm-motion-control/id504386929?ls=1&mt=8) is an iPhone app which supports gestures

## Functional Specification

The clock is made up of:

* Case
    * Craft plywood / acrylic

* Digit display
    * 7-segment LED display, 4 digits
    * LED HourDigits and MinuteDigits separator - ":", which blinks every second

* 3 buttons:
    * Capacitive, push-button
    * TimeSet: 
        * On pressing it, the HourDigits start blinking indicating it can be changed using the TimeUp and TimeDown buttons 
        * Pressing it again causes the MinuteDigits to start blinking
        * Pressing it again causes the blinking to stop and the time is updated   
    * TimeUp: 
        Increments the hour or minute (whichever is blinking)
    * TimeDown:
        Decrements the hour or minute
        
* Ambient light sensor
    * Phototransistor

* Motion detector
    * Infra-red, ultrasound

* RealTimeClock with battery backup 
    * Software implementation
    * RTC chip
 
* (Optional) Power cord with 3-pin 5amp plug

* (Optional) Battery power indicator

### Use Cases

* When powered on for the first time, it should display 12:00 and enter TimeChange mode (start blinking the HourDigits) 
* Should allow the user to change the time using the TimeSet, TimeUp and TimeDown buttons
    * In TimeChange mode, if the user input is not complete, should revert to the previous time after 10 seconds
* Should maintain accurate time
* When ambient light drops below a threshold, should turn the display off
* When display is off and motion is detected, should turn the display off for 5 seconds 
* When wall-power is turned off, should switch to battery power and continue operating normally

## System Design


## Material Required


## Cost


## Tasks

     
* Prototype motion detector
    * Ultrasound (or PIR / Infrared emitter-receiver / Xband doppler for motion detection) 
    * watch Neil's explanation
    * mill board
    * stuff board
    * test

* Prototype RTClock
    * [AVR RTC note](http://www.atmel.com/images/atmel-1259-real-time-clock-rtc-using-the-asynchronous-timer_ap-note_avr134.pdf)
    * [DS1307 board with battery backup](http://www.amazon.in/DS1307-module-Arduino-MSP430-raspberry/dp/B0180GI57A?tag=googinhydr18418-21&tag=googinkenshoo-21&ascsubtag=c1eae2c9-db9c-4481-baf2-f4eee1992dea)

* Test capacitive touch button (~3hr)
    * watch Neil's explanation in video 
    * mill board (1hr)
    * stuff board (1hr)
    * test, also with craft plywood covering the copper plate (0.5hr)
     
* Test serial network between 3 boards (3hr) 
    * program node 1 
    * program node 2
    * program controller
    * test

* Test brightness of LEDs with acrylic sheet
    * cut 7-segment digit shape in plywood and acrylic
    * check press-fit tolerance
    * check brightness

* Design clock case
    * test cut for flexures and side to panel press-fit joint
    * front face with digits, button 
    * back panel
    * side with flexures

* Create boards
    * Controller
    
    * Ambient sensor
        * Use existing board?
        
    * Motion sensor
    
    * RT Clock
        
    * 4 x LED digits
        
    * Digits separator

    * Buttons
    
    
## Schedule


## Questions

* XBand doppler sensor for long-range motion detection?

## Shopping List

* HCSR04 Ultrasonic module
* RTC DS1307 board - model / spec???
* LED 7-segment displays
* push-button with long switch
* FTDI headers
* Jumper cables: m-f, m-m, f-f
* miniUSB-USB cable

* Arduino Uno
* IR transceiver - model / spec???

* Battery + holder  
    * button cell + holder
    * 9V + holder

* XBand doppler - model / spec???
* Brighter LEDs - model / spec???


## Evaluation Criteria


## Notes

* [Paper: Hand gesture recognition using Ultrasonic sensor](http://esatjournals.net/ijret/2014v03/i06/IJRET20140306107.pdf)