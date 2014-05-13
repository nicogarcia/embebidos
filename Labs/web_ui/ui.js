
$(function(){

	// Temperature variables
	var modo_actual = 0;
	var TEMP_ACTUAL = 0;
	var TEMP_MAXIMA = 1;
	var TEMP_MINIMA = 2;
	var TEMP_PROMEDIO = 3;

	var temperatures = new Array(4);

	var temp_names = [ "temp_actual", "temp_maxima", "temp_minima", "temp_promedio"];
	var mode_names = [ "Actual", "Máxima", "Mínima", "Promedio"];

	// History array
	var history = [{label: "Actual", data: []}, {label: "Máxima", data: []},
			{label: "Mínima", data: []}, {label: "Promedio", data: []}];
	var HISTORY_LENGTH = 461;

	// Plot object
	var plot = $.plot("#placeholder", history, {
		series: {
			shadowSize: 0	// Drawing is faster without shadows
		},
		yaxis: {
			min: 0
		},
		xaxis: {
			show: false
		},
		legend: {
			position: "nw"
		}
	});

	function updatePlot(){
		plot.setupGrid();
		plot.draw();

		setInterval(updatePlot(), 300);
	}

	//updatePlot();

	// WebSocket connection object
	var connection = new WebSocket('ws://localhost:8887');
	connection.binaryType = "arraybuffer";
	connection.onmessage = recieveMessage;

	connection.onopen = function(){
	   /*Send a small message to the console once the connection is established */
	   console.log('Connection open!');
	}

	function recieveMessage(e){
	   var server_message = new Uint8Array(e.data);

	   modo_actual = ((server_message[0] << 8) | server_message[1]) - 10;

	   for(var i = 0; i < 4; i++)
	   	$('#button_' + temp_names[i]).removeClass('btn-primary').addClass('btn-default').removeClass('active');

	   $('#button_' + temp_names[modo_actual]).removeClass('btn-default').addClass('btn-primary').addClass('active');


	   for(var i = 1; i < 5; ++i){
		   temperatures[i - 1] = server_message[2 * i] + (Math.floor(server_message[2 * i + 1] / 10)) / 10;
		   $("#" + temp_names[i - 1]).text(parseFloat(temperatures[i - 1]).toFixed(1));

		   history[i - 1]["data"].push([e.timeStamp, temperatures[i - 1]]);
		}

	   	if(history[0]["data"].length > HISTORY_LENGTH){
	   		history[0]["data"].shift();
	   		history[1]["data"].shift();
	   		history[2]["data"].shift();
	   		history[3]["data"].shift();
		}

		if(temperatures[TEMP_ACTUAL] > 60)
			$('#temp_alert').removeClass('hidden');
		else
			$('#temp_alert').removeClass('hidden').addClass('hidden');

	   plot.setData(history);

	   plot.setupGrid();

	   plot.draw();

	}

	function sendMode(mode){
		var message = new Uint8Array(10);

		message[0] = (mode >> 8);
		message[1] = mode;

		connection.send(message.buffer);
	}
	
	$('#button_temp_actual').on('click', function (e) {
		sendMode(10);
	});
	
	$('#button_temp_maxima').on('click', function (e) {
		sendMode(11);
	});

	$('#button_temp_minima').on('click', function (e) {
		sendMode(12);
	});

	$('#button_temp_promedio').on('click', function (e) {
		sendMode(13);
	});
});