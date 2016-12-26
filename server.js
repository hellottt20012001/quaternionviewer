#!/usr/bin/node

const   PORT            = 8000;
var     SERIALPORT      = "/dev/ttyACM0";

/* command line arguments */
process.argv.forEach(function (val, index, array) {
    if (val == '-n') { SERIALPORT = "/dev/pts/1"; }
});

var console = require('console');
var http = require("http");
var url = require("url");
var fs = require("fs");
var path = require("path");
var WebSocketServer = require('websocket').server;

/* implement simple HTTP server */
function onRequest(request, response)
{
    var pathname = url.parse(request.url).pathname;
    var query = url.parse(request.url).query;

    if (pathname == '/') pathname += 'index.html';
    var filePath = "." + pathname;

    var extname = path.extname(filePath);
    var contentType = 'text/html';
    switch (extname)
    {
        case '.js':
            contentType = 'text/javascript';
            break;
        case '.css':
            contentType = 'text/css';
            break;
        case '.json':
            contentType = 'application/json';
            break;
        case '.png':
            contentType = 'image/png';
            break;
        case '.jpg':
            contentType = 'image/jpg';
            break;
    }

    fs.readFile(filePath, function(error, content) {
        if (error)
        {
            response.writeHead(500);
            response.end("Error: " + error.code + "\n");
            response.end();
        }
        else
        {
            response.writeHead(200, { 'Content-Type': contentType });
            response.end(content, 'utf-8');
        }
    });
}

var server = http.createServer(onRequest).listen(PORT, function() { /*console.log("Server listening on port %s", PORT);*/ });

/* WebSocket server */

var wsServer = new WebSocketServer({ httpServer: server, autoAcceptConnections: false });

function onWsConnMessage(message)
{
    if (message.type == 'utf8')
        serial.write(message.utf8Data);
    //     console.log('Received message: ' + message.utf8Data);
    // else if (message.type == 'binary')
    //     console.log('Received binary data.');
}

function onWsConnClose(reasonCode, description)
{
    // console.log( "Peer disconnected with reason: " + reasonCode );
}

var connections = [];
function onWsRequest(request)
{
    var connection = request.accept(null, request.origin);
    // console.log( "WebSocket connection accepted." );

    connection.on('message', onWsConnMessage);
    connection.on('close', onWsConnClose);
}

function send(data)
{
    for (var i = 0; i < connections.length; i++)
        connections[i].send(data);
}

var connected = false;
function onWsConnect(connection)
{
    connections.push(connection);
    connected = true;
}

wsServer.on('request', onWsRequest);
wsServer.on('connect', onWsConnect);

/* Serial port */

var SerialPort = require('serialport');

var serial = new SerialPort(SERIALPORT, { baudRate: 115200, lock: false, parser: SerialPort.parsers.byteLength(1) });

serial.on('data', function (data) {
        if (connected) send(data);
        process.stdout.write(data.toString()); /* pass data from serial to stdout */
});

/* pass stdin to serial port */
process.stdin.setRawMode(true);     // without this, we would only get streams once enter is pressed
process.stdin.resume();             // resume stdin in the parent process (node app won't quit all by itself unless an error or process.exit() happens)
process.stdin.setEncoding('utf8');  // i don't want binary, do you?
process.stdin.on( 'data', function(key) {   // on any data into stdin
    if (key === '\u0003') // ctrl-c ( end of text )
        process.exit();
    serial.write(key);
});

console.log('\033[2J');
