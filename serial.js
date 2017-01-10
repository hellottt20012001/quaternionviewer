#!/usr/bin/node

var SERIALPORT = "/dev/pts/1";
//var SERIALPORT = "COM3";

var SerialPort = require('serialport');

var serial = new SerialPort(SERIALPORT, { baudRate: 115200, lock: false, parser: SerialPort.parsers.byteLength(1) });

serial.on('data', function (data) {
    process.stdout.write(data.toString()); /* pass data from serial to stdout */
});

/* pass stdin to serial port */
process.stdin.setRawMode(true);     // without this, we would only get streams once enter is pressed
process.stdin.resume();             // resume stdin in the parent process (node app won't quit all by itself unless an error or process.exit() happens)
process.stdin.setEncoding('utf8');  // i don't want binary, do you?
process.stdin.on( 'data', function(key) {   // on any data into stdin
    if (key === '\u0003') process.exit(); // ctrl-c ( end of text )
    process.stdout.write(key);
    serial.write(key); // pass stdin to serial
});
