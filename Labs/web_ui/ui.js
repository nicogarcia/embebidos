
$(function(){

	// Message types
	var SERIAL_SELECTION = 40;
	var AV_COM_SEND = 60;

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

	// i2c Mode
	var i2c_mode;
	var i2c_mode_names = [ "", "MASTER", "SLAVE"];

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

	// WebSocket connection object
	var connection_string = 'ws://localhost:8887';
	var connection;

	startConnection();

	function startConnection(){
		connection = new WebSocket(connection_string);
		connection.binaryType = "arraybuffer";
		connection.onmessage = recieveMessage;
		connection.onclose = onCloseWS;
		connection.onopen = onOpenWS;
	}

	function onCloseWS(){
		console.log('Connection opened!');
		setTimeout(restartConnection, 100);
	};

	function onOpenWS(){
		console.log('Connection opened!');
	}

	function restartConnection(){
		connection = new WebSocket(connection_string);
		connection.binaryType = "arraybuffer";
		connection.onmessage = recieveMessage;
		connection.onclose = onCloseWS;
		connection.onopen = onOpenWS;
	}

	function recieveMessage(e){
		var server_message = new Uint8Array(e.data);

		if(server_message[0] == AV_COM_SEND)
			parseSerialSelection(server_message);
		else 
			parseTempMessage(server_message, e.timeStamp);

	}

	function parseSerialSelection(server_message){
		var select_box = $('#com_select');
		select_box.empty();
		// Create dropdown options
		for(var i = 1; i < 10; ++i){
			if(server_message[i] != 0){
				select_box.append("<option value=" + i + ">COM" + i + "</option>");
			}
		}
		if(select_box.val() != null)
			selectPort(parseInt(select_box.val()));
	}

	function parseTempMessage(server_message, timeStamp){
		i2c_mode = server_message[0];
		$('#i2c_mode').text(i2c_mode_names[i2c_mode]);

		modo_actual = server_message[1] - 10;

		// Highlight current mode button
		for(var i = 0; i < 4; i++)
			$('#button_' + temp_names[i]).removeClass('btn-primary').addClass('btn-default').removeClass('active');

		$('#button_' + temp_names[modo_actual]).removeClass('btn-default').addClass('btn-primary').addClass('active');


		// Read temperatures on temperatures, and add them to history
		for(var i = 1; i < 5; ++i){
			temperatures[i - 1] = server_message[2 * i] + (Math.floor(server_message[2 * i + 1] / 10)) / 10;
			$("#" + temp_names[i - 1]).text(parseFloat(temperatures[i - 1]).toFixed(1));

			history[i - 1]["data"].push([timeStamp, temperatures[i - 1]]);
		}

		// Displace first record when history is full
		if(history[0]["data"].length > HISTORY_LENGTH){
			history[0]["data"].shift();
			history[1]["data"].shift();
			history[2]["data"].shift();
			history[3]["data"].shift();
		}

		// if current temp is greater than 60 show alert
		if(temperatures[TEMP_ACTUAL] > 60)
			$('#temp_alert').removeClass('hidden');
		else
			$('#temp_alert').removeClass('hidden').addClass('hidden');

		// Set data, recalculate grid and draw plot
		plot.setData(history);
		plot.setupGrid();
		plot.draw();
	}

	function selectPort(port){
		var message = new Uint8Array(10);
		message[0] = SERIAL_SELECTION + port;
		message[9] = SERIAL_SELECTION + port;

		connection.send(message.buffer);
	}

	function sendMode(mode){
		var message = new Uint8Array(10);

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

	$('#com_select').change(function(e) { 
		var selected_com = parseInt($('#com_select').val());

		selectPort(selected_com);
	});
});