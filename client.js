"use strict";

/* websocket */
var wsClient;

function main()
{
	var wsClient = new WsClient();
	wsClient.connect();

	var setRotation = Module.cwrap('setRotation', null, ['number', 'number', 'number', 'number']);
	//setRotation(0, 0, 1, 1);
}

function update()
{
	setRotation(w, x, y, z);
	console.log(w, x, y, z);
}
