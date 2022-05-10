var queue = new CBuffer(10);

var eps = 0.9       // filter time constant
var filter = 0.0    // filtered value
var nloop = 100.0   // number of loops accumulated
var amp = 25.0      // difference amplitude

function logMsg(msg) {
    messages.value = msg + "\n" + messages.value; 
}

function now() {
    return new Date().getTime();
}

var minFilter = 100000;
var maxFilter = -100000;

var minFilterTime = now();
var maxFilterTime = now();

function millisPassedSince(since, ms) {
    return (since + ms) <= now();
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
    
    // calculate the values sent by the sensor board
    onLow = queue.shift();
    onHigh = queue.shift();
    onValue = (256 * onHigh + onLow) / nloop;
    
    offLow = queue.shift();
    offHigh = queue.shift();
    offValue = (256 * offHigh + offLow) / nloop;
    
    filter = Math.round((1 - eps) * filter + eps * amp * (onValue - offValue));

    // Calculate thresholds for up and down movement based on the diff between the hi and lo value.
    
    // Respond to changing ambient light by calculating the up & down threshold based on the range of values seen recently.  
//    if (filter < minFilter || millisPassedSince(minFilterTime, 1000)) {
    if (filter < minFilter) {
        console.log("changing minFilter");
        minFilter = filter;
        minFilterTime = now();
    }
    
//    if (filter > maxFilter || millisPassedSince(maxFilterTime, 1000)) {
    if (filter > maxFilter) {
        console.log("changing maxFilter");
        maxFilter = filter;
        maxFilterTime = now();
    }
    
    range = maxFilter - minFilter;
    downThreshold = Math.round(minFilter + (range * 5/16));
    upThreshold = Math.round(maxFilter - (range * 9/16));
     
    // Appy the threshold and set the plane's Y position 
    if (filter <= downThreshold && mousePos.y > -1) {
        mousePos.y -= 0.1;
    } else if (filter > upThreshold && mousePos.y < 1) {
        mousePos.y += 0.1;
    }
    console.log("range: " + range + " filter: " + filter + " downThreshold: " + downThreshold + " upThreshold: " + upThreshold + " mousePos: x " + mousePos.x + ", y " + mousePos.y);
};

function setupSerialComms() {
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
}
