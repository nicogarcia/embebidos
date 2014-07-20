package lab4processing.V2;

import java.net.UnknownHostException;
import java.util.Arrays;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.java_websocket.WebSocket;

import processing.core.PApplet;
import processing.serial.Serial;

public class MainApplet extends PApplet {

	private static MainApplet instance = new MainApplet();

	public static MainApplet Instance() {
		return instance;
	}

	// Data Structures
	static Map<String, Serial> serial_ports = new HashMap<String, Serial>(10);
	static Map<WebSocket, Serial> ws_to_serial = new HashMap<WebSocket, Serial>();
	static Map<Serial, HashSet<WebSocket>> serial_to_websockets = new HashMap<Serial, HashSet<WebSocket>>();

	// GUI Server
	GUI_Server_v2 gui_server_ws;

	boolean waiting = false;
	String to_wait = new String();
	Serial serial_to_pass;
	String s_name;

	// Serial port list update task
	Runnable updateSerialPorts = new Runnable() {
		public void run() {
			try {
				while (true) {
					Set<String> new_ports;

					// Calculate new ports
					new_ports = new HashSet<String>(
							Arrays.asList(Serial.list()));

					// Remove existing ports
					new_ports.removeAll(serial_ports.keySet());

					for (String serial_name : new_ports) {
						// Open serial port
						s_name = "COM1";
						waiting = true;
						synchronized (to_wait) {
							to_wait.wait();
						}
						waiting = false;
						Serial current = serial_to_pass;

						// Set Serial properties
						current.clear();
						current.bufferUntil(END_TOKEN);

						serial_ports.put(serial_name, current);
					}

					// Calculate closed ports
					Set<String> current_ports = new HashSet<String>(
							Arrays.asList(Serial.list()));

					// Remove current ports from registered ports, remainder
					// ports should be closed
					Collection<Serial> closed_ports = new HashSet<Serial>(
							serial_ports.values());
					closed_ports.removeAll(current_ports);

					for (Serial serial : closed_ports) {
						serial.clear();
						serial.stop();
					}

					// Wait before next check
					Thread.sleep(500);
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	};

	public void setup() {
		reading_msg = new byte[MSG_LENGTH];
		writing_msg = new byte[MSG_LENGTH];

		try {
			gui_server_ws = new GUI_Server_v2(8887, this);
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
		gui_server_ws.start();

		// new Thread(updateSerialPorts).start();
	}

	public void draw() {
		size(1, 1);
		try {
			Set<String> new_ports_names;

			// Calculate new ports
			new_ports_names = new HashSet<String>(Arrays.asList(Serial.list()));

			// Print all COMs
			/*
			 * for (String s : Serial.list()) System.out.print(s + " - ");
			 * System.out.println();
			 */

			// Remove existing ports
			new_ports_names.removeAll(serial_ports.keySet());

			for (String serial_name : new_ports_names) {

				// Open serial port
				Serial current = new Serial(this, serial_name, 115200);
				// Set Serial properties
				current.clear();
				current.bufferUntil(END_TOKEN);

				serial_ports.put(serial_name, current);
				serial_to_websockets.put(current, new HashSet<WebSocket>());
			}

			// Calculate closed ports
			Set<String> current_ports_names = new HashSet<String>(
					Arrays.asList(Serial.list()));

			// Remove current ports from registered ports, remainder
			// ports should be closed
			Set<String> closed_ports_names = new HashSet<String>(
					serial_ports.keySet());

			closed_ports_names.removeAll(current_ports_names);

			for (String serial_name : closed_ports_names) {
				Serial serial = serial_ports.remove(serial_name);

				// Detach serial to ws and ws to serial
				HashSet<WebSocket> to_detach = serial_to_websockets
						.remove(serial);
				for (WebSocket webSocket : to_detach) {
					ws_to_serial.remove(webSocket);
					System.out.println("Detached " + serial_name);
				}

				serial.clear();
				serial.stop();
			}

			if (new_ports_names.size() + closed_ports_names.size() > 0)
				// Send currently available ports to all WebSockets
				gui_server_ws.sendAvailableSerialToAll();

			// Wait before next check
			Thread.sleep(500);

		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	/*** MESSAGE ***/
	// Message length without tokens
	final int MSG_LENGTH = 10;

	// Start and end tokens
	final char START_TOKEN = 110;
	final char END_TOKEN = 120;

	// Number padding to map, for ex. 0 => 100, 1 => 101
	// This is to avoid getting false numbers, like 0
	final char NUMBER_PADDING = 0;

	// Temporary arrays to read and write message
	static byte[] reading_msg;
	static byte[] writing_msg;

	// Modes values (from 5 available)
	final int MODO_TEMP_ACTUAL = 10;
	final int MODO_TEMP_MAXIMA = 11;
	final int MODO_TEMP_MINIMA = 12;
	final int MODO_TEMP_PROMEDIO = 13;

	int mode = MODO_TEMP_ACTUAL;
	double tempActual;
	double tempMax;
	double tempMin;
	double tempProm;
	byte current_byte;

	void ParseMessage() {
		// Parse mode
		mode = (reading_msg[0] << 8) | reading_msg[1];

		// Make 2-Byte to Double precision conversion
		double[] temps = new double[4];
		for (int i = 1; i < 5; i++) {
			temps[i - 1] = reading_msg[2 * i] - NUMBER_PADDING;
			temps[i - 1] += (reading_msg[2 * i + 1] - NUMBER_PADDING) / 100.0;
		}

		// Build struct to be returned
		tempActual = temps[0];
		tempMax = temps[1];
		tempMin = temps[2];
		tempProm = temps[3];
	}

	void ReadMessage(Serial port) {
		// Leemos mensaje
		int msg_pointer = 0;
		while (true) {
			if (port.available() == 0)
				continue;
			current_byte = (byte) port.read();
			if (current_byte == START_TOKEN) {
				msg_pointer = 0;
				// print("S ");
				continue;
			}
			// End token read
			if (current_byte == END_TOKEN) {
				// REMOVED: Additional check, is pointer at the end?
				// println("E");
				ParseMessage();
				break;
			}

			// Number read
			// println(reading_msg[msg_pointer]);
			if (msg_pointer == MSG_LENGTH) {
				println("msg overflow");
				break;
			}
			reading_msg[msg_pointer++] = current_byte;

			// print(current_byte + " ");
		}
	}

	void sendMessage(Serial port, byte[] array) {
		// Write start token
		port.write(START_TOKEN);

		// Write message
		for (int i = 0; i < MSG_LENGTH; i++)
			port.write(array[i]);

		// Write end token
		port.write(END_TOKEN);
	}

	public void serialEvent(Serial p) {
		ReadMessage(p);

		Collection<WebSocket> web_sockets = serial_to_websockets.get(p);
		gui_server_ws.sendToAll(reading_msg, web_sockets);
	}

}
