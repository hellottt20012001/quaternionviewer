"use strict";

var reader = new FileReader();

/* Class WsClient */
var WsClient = function()
{
	var ws;
	var url = "ws://" + window.location.href.replace("http://", "") + ":8000";
	var connected = false;

	this.connect = function ()
	{
		if ("WebSocket" in window)
			ws = new WebSocket(url);
		else if ("MozWebSocket" in window)
			ws = new MozWebSocket(url);

		ws.onopen = function(e)  { connected = true; console.log("Connected to websocket server"); };
		ws.onerror = function(e) { console.log(e); };
		ws.onclose = function(e) { connected = false; console.log(e.code+(e.reason != "" ? ","+e.reason : ""));};

		ws.onmessage = function(e)
		{
			console.log(e.data);
			/* update parameters from serial */
			var params = e.data.split(',');
			for (var i=0; i<params.length; i++)
			{
				var param = params[i].split('=');
				window["_" + param[0]] = param[1];
			}
			/* update client */
			update();
		};
	};

	/* Class methods */
	this.disconnect = function() { ws.close(); };
	this.send = function(msg) { ws.send(msg); };
}
