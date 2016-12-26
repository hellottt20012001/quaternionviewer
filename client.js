"use strict";

/* settings */
var time_tick = 1.0;
var iload_max = 12;
var iload_tick = 0.5;
var temp_max = 1200;
var temp_tick = 50;

/* DOM elements */
var gd;

/* websocket */
var wsClient;

/* plot data */
var data;
var setpointData, tempData, iloadData;

var edit_mode = false;

function pid_main()
{
	gd 			= document.getElementById("gd");

	/* init websocket */
	wsClient = new WsClient();
	wsClient.connect();

	var layout = {
		margin: {
		   l: 50,
		   r: 50,
		   b: 50,
		   t: 50,
		   pad: 1
		},
	    xaxis: {
	        dtick: time_tick,
	    },
	    yaxis: {
			title: 'Temperature',
	        range: [0, temp_max],
			dtick: temp_tick,
			fixedrange: true
	    },
		yaxis2: {
		   title: 'Load current',
		   range: [0, iload_max],
		   dtick: iload_tick,
		   titlefont: {color: 'rgb(148, 103, 189)'},
		   tickfont: {color: 'rgb(148, 103, 189)'},
		   overlaying: 'y',
		   side: 'right'
	 	}
    };

	setpointData = {
		  x: [],
		  y: [],
		  type: 'scatter',
		  mode: 'none',
		  fill: 'tonexty',
		  name: 'Setpoint',
		  connectgaps: true
		};

	tempData = {
		  x: [],
		  y: [],
		  type: 'scatter',
		  mode: 'lines',
		  fill: 'tozeroy',
		  name: 'Temperature',
		  connectgaps: true
		};

	iloadData = {
		  x: [],
		  y: [],
		  type: 'scatter',
		  mode: 'lines',
		  name: 'Load current',
		  connectgaps: true,
		  yaxis: 'y2'
		};

	var data = [setpointData, tempData, iloadData];

	Plotly.newPlot(gd, data, layout);

	window.onresize = function() {
	    Plotly.Plots.resize(gd);
	};
}

function update ()
{
	document.getElementById("tempDisplay").innerHTML = Number(avg_temp).toFixed(1) + " °C";
	if (avg_temp < setpoint) tempDisplay.style.color = "#4B6F44";
	else tempDisplay.style.color = "#B31B1B";

	document.getElementById("setpointDisplay").innerHTML 	= Number(setpoint).toFixed(1) + " °C";
	document.getElementById("iloadDisplay").innerHTML 		= Number(avg_iload).toFixed(2) + " A";

	document.getElementById("outputP").innerHTML = Number(outputP).toFixed(1) + "%";
	document.getElementById("outputI").innerHTML = Number(outputI).toFixed(1) + "%";
	document.getElementById("outputD").innerHTML = Number(outputD).toFixed(1) + "%";
	document.getElementById("output").innerHTML = Number(output).toFixed(1) + "%";

	document.getElementById("percent").innerHTML = Number(percent).toFixed(1) + "%";

	if (pwm_on == 1) document.getElementById("pwm_on").checked = true;
	if (pwm_on == 0) document.getElementById("pwm_on").checked = false;

	if (!edit_mode)
	{
		document.getElementById("tempSlope").value 		= Number(temp_slope);
		document.getElementById("tempOffset").value 	= Number(temp_offset);
		document.getElementById("iloadSlope").value 	= Number(iload_slope);

		document.getElementById("kp").value 	= Number(Kp);
		document.getElementById("ti").value 	= Number(Ti);
		document.getElementById("td").value 	= Number(Td);
	}

	tempData.x.push(second/60.0);
	tempData.y.push(avg_temp);
	setpointData.x.push(second/60.0);
	setpointData.y.push(setpoint);
	iloadData.x.push(second/60.0);
	iloadData.y.push(avg_iload);
	Plotly.redraw(gd)
}

function pwmClick (o)
{
	wsClient.send("<pwm_on=" + (o.checked ? "1" : "0") + ">");
}

function toggle_edit_mode (enabled)
{
	document.getElementById("tempSlope").disabled 	= !enabled;
	document.getElementById("tempOffset").disabled 	= !enabled;
	document.getElementById("iloadSlope").disabled 	= !enabled;

	document.getElementById("kp").disabled 	= !enabled;
	document.getElementById("ti").disabled 	= !enabled;
	document.getElementById("td").disabled 	= !enabled;

	document.getElementById("applyButton").disabled = !enabled;
	document.getElementById("saveButton").disabled 	= !enabled;
	document.getElementById("edit_mode").checked 	= enabled;
	edit_mode = enabled;
}

function apply_params()
{
	wsClient.send("<temp_slope=" + document.getElementById("tempSlope").value + ">");
	wsClient.send("<temp_offset=" + document.getElementById("tempOffset").value + ">");
	wsClient.send("<iload_slope=" + document.getElementById("iloadSlope").value + ">");
	wsClient.send("<Kp=" + document.getElementById("kp").value + ">");
	wsClient.send("<Ti=" + document.getElementById("ti").value + ">");
	wsClient.send("<Td=" + document.getElementById("td").value + ">");
	toggle_edit_mode (false);
}

function save_params()
{
	wsClient.send("<save>");
}
