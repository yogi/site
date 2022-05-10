---
layout: page
title: Week 17 - Interface & Application Programming
permalink: w17-interface.html
---

[Class Notes](http://academy.cba.mit.edu/classes/interface_application_programming/index.html)   

## Assignment

> write an application that interfaces with an input &/or output device that you made,
    comparing as many tool options as possible
    
Looking at all the interface options, these seem interesting: 

* Javascript: because it seems to be the way all interfaces are going. Also would be interesting to play with serial 
    communications from the browser.
* WebGL, Three.js: 3D programming in the browser seems super interesting.

Given the above, I started researching what I could build. 

I came across a game called the [Aviator](http://tympanus.net/codrops/2016/04/26/the-aviator-animating-basic-3d-scene-threejs/), 
    which is written in Three.js.

I've been wanting to get into game development and physics engines, so this would be a good chance to do so.

So, I'm going to create a simple game that involves a character continuously moving forward, where you can control its vertical movement.
  For this I'll use the ambient reflection detecting sensor to control the up and down movements.
   
## Plan

*  Iteration 1
    * Write a simple Javascript program to accept input from the board and display it on the screen. 
    * Translate the sensor readings to an up or down command.
* Iteration 2
    * Create a simple 3D world and a character moving continuously from left to right. Use the sensor inputs to move the character up and down.

## Iteration 1

### Javascript program to accept input from the board
 
I used the code from the input-device week to check that the sensor is working fine. The python app showed the values correctly.
 
Next I have to create a ChromeApp in Javascript. 

I followed the instructions on the [Chrome App Developer page](https://developer.chrome.com/apps/first_app) and staretd customizing 
    the [hello-world sample](https://github.com/GoogleChrome/chrome-app-samples/tree/master/samples/hello-world) app. 
 
Next I need to accept input from the serial interface. Here's a good [tutorial](http://renaun.com/blog/2013/05/using-the-chrome-serial-api-with-arduino/) 
    on using Chrome's serial api, and the [api documentation](https://developer.chrome.com/apps/serial) itself.

Here's a simple JS program that outputs the numbers received over serial to console:
 
<pre>
<code class="language-javascript">
var queue = new CBuffer(10);

var eps = 0.9       // filter time constant
var filter = 0.0    // filtered value
var nloop = 100.0   // number of loops accumulated
var amp = 25.0      // difference amplitude

function logMsg(msg) {
    messages.value = msg + "\n" + messages.value; 
}

function onReceive(info) {
    var view = new Uint8Array(info.data);
    for (i = 0; i < info.data.byteLength; i++) {
        queue.push(view[i]);
    }
    
    if (queue.size < 8) return; 
    if (queue.shift() != 1) return;
    if (queue.shift() != 2) return;
    if (queue.shift() != 3) return;
    if (queue.shift() != 4) return;
    
    onLow = queue.shift();
    onHigh = queue.shift();
    onValue = (256 * onHigh + onLow) / nloop;
    
    offLow = queue.shift();
    offHigh = queue.shift();
    offValue = (256 * offHigh + offLow) / nloop;
    
    filter = (1 - eps) * filter + eps * amp * (onValue - offValue);

    console.log(filter + " | " + onValue + " | " + offValue);
};

onload = function() {
    chrome.serial.getDevices(function(ports) {
        var eligiblePorts = ports.filter(function(port) {
            return !port.path.match(/[Bb]luetooth/) && port.path.match(/\/dev\/tty/);
        });
        
        if (eligiblePorts.length < 1) {
            logMsg("No ports found");
            return;
        } else if (eligiblePorts.length > 1) {
            logMsg("More than 1 port found, connecting to the first one: " + eligiblePorts[0].path);
        } else {
            logMsg("Connecting to " + eligiblePorts[0].path);
        }
        
        port = eligiblePorts[0];
        
        chrome.serial.connect(port.path, function(connInfo) {
            logMsg("Connected to " + port.path);

            chrome.serial.onReceive.addListener(onReceive);
        });
    });
};
</code>
</pre>

### Translate the sensor readings to an up or down command

This step would be better done after I have a basic 3D world created.

## Iteration 2

### Simple 3D World

Next I created a Chrome App using the code from the Aviator demo. 

I changed the sea surface to red and created a rocket using a combination of simple shapes, here's the relevant code using Three.js:

<pre>
<code class="language-javascript">
var Rocket = function() {
    this.mesh = new THREE.Object3D();
    this.mesh.name = "airPlane";

    rocketColor = Colors.white;

    // Create the body
    var geomBody = new THREE.BoxGeometry(160, 50, 50, 1, 1, 1);
    var matBody = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var body = new THREE.Mesh(geomBody, matBody);
    body.castShadow = true;
    body.receiveShadow = true;
    this.mesh.add(body);

    // Create nose
    var geomNose = new THREE.ConeGeometry(35.55, 100, 4);
    var matNose = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var nose = new THREE.Mesh(geomNose, matNose);
    nose.position.x = 129;
    nose.castShadow = true;
    nose.receiveShadow = true;
    nose.rotation.x = Math.PI / 4;
    nose.rotation.z = Math.PI * 3 / 2;
    this.mesh.add(nose);

    // Create Fins

    var geomFin1 = new THREE.BoxGeometry(60, 100, 15, 1, 1, 1);
    var matFin1 = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var fin1 = new THREE.Mesh(geomFin1, matFin1);
    fin1.position.set(-80, 0, 0);
    fin1.rotation.x = Math.PI / 4;
    fin1.castShadow = true;
    fin1.receiveShadow = true;
    this.mesh.add(fin1);

    var geomFin2 = new THREE.BoxGeometry(60, 100, 15, 1, 1, 1);
    var matFin2 = new THREE.MeshPhongMaterial({
        color: rocketColor,
        shading: THREE.FlatShading
    });
    var fin2 = new THREE.Mesh(geomFin2, matFin2);
    fin2.position.set(-80, 0, 0);
    fin2.rotation.x = Math.PI * 3 / 4;
    fin2.castShadow = true;
    fin2.receiveShadow = true;
    this.mesh.add(fin2);
};
</code>
</pre>

 
I then modified the onReceive function to translate the sensor values into changes to the Y position of the rocket. 

Here is the updated code:

<pre>
<code class="language-javascript">
function onReceive(info) {
    var view = new Uint8Array(info.data);
    for (i = 0; i < info.data.byteLength; i++) {
        queue.push(view[i]);
    }
    
    if (queue.size < 8) return; 
    if (queue.shift() != 1) return;
    if (queue.shift() != 2) return;
    if (queue.shift() != 3) return;
    if (queue.shift() != 4) return;
    
    // read the values sent by the sensor board
    onLow = queue.shift();
    onHigh = queue.shift();
    onValue = (256 * onHigh + onLow) / nloop;
    
    offLow = queue.shift();
    offHigh = queue.shift();
    offValue = (256 * offHigh + offLow) / nloop;
    
    filter = (1 - eps) * filter + eps * amp * (onValue - offValue);

    // Calculate thresholds for up and down movement based on the diff between the hi and lo value.
    
    // Respond to changing ambient light by calculating the up & down threshold based on the range of values seen recently.  
    if (filter < minFilter || millisPassedSince(minFilterTime, 10)) {
        minFilter = filter;
        minFilterTime = now();
    }
    
    if (filter > maxFilter || millisPassedSince(maxFilterTime, 10)) {
        maxFilter = filter;
        maxFilterTime = now();
    }
    
    range = maxFilter - minFilter;
    downThreshold = minFilter + (range * 5/16);
    upThreshold = maxFilter - (range * 9/16);
     
    // Appy the threshold and set the rocket's Y position 
    if (filter <= downThreshold && mousePos.y > -1) {
        mousePos.y -= 0.1;
    } else if (filter > upThreshold && mousePos.y < 1) {
        mousePos.y += 0.1;
    }
    console.log("filter: " + filter + " downThreshold: " + downThreshold + " upThreshold: " + upThreshold + "; mousePos: x " + mousePos.x + ", y " + mousePos.y);
};
</code>
</pre>

Here's a video showing the result:

<video controls>
  <source src="images/w17-aviator.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>

## Original Files:

* Full source: [mission-to-mars.zip](files/w17/mission-to-mars.zip)
