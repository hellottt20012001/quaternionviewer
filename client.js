"use strict";

/* websocket */
var wsClient;
var setRotation;
var _x = 0, _y = 0, _z = 0, _w = 1;
var quatdisplay;

function update()
{
	setRotation(_w, _x, _y, _z);
	quatdisplay.innerHTML = _w + " " + _x + "i " + _y + "j " + _z + "k";
}

function main()
{
	quatdisplay = document.getElementById("quatdisplay");
	wsClient = new WsClient();
	wsClient.connect();

	setRotation = Module.cwrap('setRotation', null, ['number', 'number', 'number', 'number']);
	update();
}
