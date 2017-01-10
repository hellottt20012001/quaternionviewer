#!/bin/bash
socat -d -d pty,raw,echo=0 pty,raw,echo= &  # set up null modem
#./server.js -n # start HTTP/websocket server and serial port
#killall socat
