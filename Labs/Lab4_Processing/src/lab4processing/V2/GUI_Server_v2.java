package lab4processing.V2;

import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.util.Collection;

import org.java_websocket.WebSocket;
import org.java_websocket.framing.Framedata;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

import processing.serial.Serial;

public class GUI_Server_v2 extends WebSocketServer {

	final byte SERIAL_SELECTION = 40;

	MainApplet owner_applet;

	/******* Data Simulation ********/
	byte last_mode = 10;
	int count = 0;
	byte temp_alert_int;

	Runnable task = new Runnable() {
		public void run() {
			while (true) {
				try {
					Thread.sleep(130);
					if (count++ % 100 < 30)
						temp_alert_int = 65;
					else
						temp_alert_int = 10;
					sendTemps(new byte[] { 0, last_mode, temp_alert_int, 3, 4,
							5, 6, 7, 8, 9 });
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}
	};

	/********************************/

	public void sendTemps(byte[] bytes) {
		if (this.connections().isEmpty())
			return;

		this.sendToAll(bytes);
	}

	public GUI_Server_v2(int port, MainApplet mainApplet)
			throws UnknownHostException {
		super(new InetSocketAddress(port));
		this.owner_applet = mainApplet;
	}

	public GUI_Server_v2(InetSocketAddress address) {
		super(address);
	}

	private MainApplet MainAppletI() {
		return MainApplet.Instance();
	}

	public void onMessage(WebSocket conn, ByteBuffer message) {
		byte[] array = message.array();

		System.out.print("Message Recieved, is Serial Selection? ");
		if (array[0] >= SERIAL_SELECTION) {
			System.out.println("Yes!");
			byte serial_id = (byte) (array[0] - SERIAL_SELECTION);
			String serial_name = "COM" + serial_id;

			// If Serial port exists and is opened, associate ws with it
			if (MainApplet.serial_ports.containsKey(serial_name)) {
				Serial serial = MainApplet.serial_ports.get(serial_name);

				// WS => serial
				MainApplet.ws_to_serial.put(conn, serial);

				// Serial =>+ WS
				MainApplet.serial_to_websockets.get(serial).add(conn);
			} else
				System.out.println("Requested serial port " + serial_name
						+ ", not available");
			return;

		} else
			System.out.println("No!");

		System.out.println("Is there an assoc serial to this ws? ");
		MainAppletI();
		Serial associated_serial = MainApplet.ws_to_serial.get(conn);
		// If this websocket has a serial associated
		if (associated_serial != null) {
			System.out.println("Yes!");
			owner_applet.sendMessage(associated_serial, array);
		}
		System.out.println("No");
	}

	public void onClose(WebSocket ws, int arg1, String arg2, boolean arg3) {
		System.out.println("connection closed");

		MainAppletI();
		// Remove websocket to serial relation
		Serial s = MainApplet.ws_to_serial.remove(ws);

		MainAppletI();
		// Remove serial to socket relation
		if (MainApplet.serial_to_websockets.get(s) != null) {
			MainAppletI();
			MainApplet.serial_to_websockets.get(s).remove(ws);
		}
	}

	public void onOpen(WebSocket arg0, ClientHandshake arg1) {
		System.out.println("New connection opened!");
	}

	public void sendToAll(byte[] bytes) {
		Collection<WebSocket> con = connections();
		synchronized (con) {
			for (WebSocket c : con) {
				c.send(bytes);
			}
		}
	}

	public void sendToAll(byte[] bytes, Collection<WebSocket> web_sockets) {
		synchronized (web_sockets) {
			for (WebSocket c : web_sockets) {
				c.send(bytes);
			}
		}
	}

	public void onFragment(WebSocket conn, Framedata fragment) {
		// System.out.println( "received fragment: " + fragment );
	}

	public void onError(WebSocket conn, Exception ex) {
		ex.printStackTrace();
		if (conn != null) {
			// some errors like port binding failed may not be assignable to a
			// specific websocket
		}
	}

	@Override
	public void onMessage(WebSocket arg0, String arg1) {
	}
}
