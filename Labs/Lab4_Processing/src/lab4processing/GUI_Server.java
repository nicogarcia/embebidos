package lab4processing;

import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.nio.ByteBuffer;
import java.util.Collection;

import org.java_websocket.WebSocket;
import org.java_websocket.framing.Framedata;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

public class GUI_Server extends WebSocketServer {

	Lab4Processing owner_applet;
	
	/******* Data Simulation ********/
	byte last_mode = 10;
	int count = 0;
	byte temp_alert_int;
	
	Runnable task = new Runnable() {
		public void run() {
			while (true) {
				try {
					Thread.sleep(130);
					if(count++ % 100 < 30)
						temp_alert_int = 65;
					else
						temp_alert_int = 10;
					sendTemps(new byte[] {0, last_mode, temp_alert_int, 3, 4, 5, 6, 7, 8, 9});
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

	public GUI_Server(int port, Lab4Processing lab4Processing) throws UnknownHostException {
		super(new InetSocketAddress(port));
		this.owner_applet = lab4Processing;
	}

	public GUI_Server(InetSocketAddress address) {
		super(address);
	}

	public void onMessage(WebSocket conn, ByteBuffer message) {
		byte[] array = message.array();

		owner_applet.mode = ((array[0] << 8) | array[1]);
		last_mode = array[1];
		System.out.println(owner_applet.mode);
		owner_applet.sendMessage();
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

	public void onClose(WebSocket arg0, int arg1, String arg2, boolean arg3) {
		System.out.println("connection closed");
	}

	public void onOpen(WebSocket arg0, ClientHandshake arg1) {
		System.out.println("connection opened");
		//new Thread(task).start();
	}

	public void sendToAll(byte[] bytes) {
		Collection<WebSocket> con = connections();
		synchronized (con) {
			for (WebSocket c : con) {
				c.send(bytes);
			}
		}
	}

	@Override
	public void onMessage(WebSocket arg0, String arg1) {
	}
}
