package lab4processing;

import processing.core.PApplet;
import processing.core.PFont;
//Importamos la librería serial
import processing.serial.*;

public class Lab4Processing extends PApplet {

	/**
	 * Ejemplo de skecth Processing para el desarrollo del Laboratorio Nº4.
	 * Sistemas Embebidos - 1º Cuatrimestre de 2012
	 * 
	 * Este skecth implementa un programa en el host que tiene la capacidad de
	 * graficar funciones que evolucionan en el tiempo y además permite la
	 * presentación de datos simples mediante Labels, y la captura de eventos
	 * del usuario mediante la implementación de botones simples.
	 * 
	 * La aplicación divide la ventana en 2 regiones, una de dibujado y otra
	 * donde se ubican los botones y etiquetas de información.
	 * 
	 * Autor: Sebastián Escarza Sistemas Embebidos - 1ºC de 2012.
	 */

	// Declaramos un objeto tipo Serial
	Serial port;

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
	int ultimo = -1;
	int mode = MODO_TEMP_ACTUAL;
	double tempActual;
	double tempMax;
	double tempMin;
	double tempProm;
	byte current_byte;

	// Declaraciones para graficar funciones...
	int cantValues;
	ScrollingFcnPlot fc_tempActual;
	ScrollingFcnPlot fc_tempMax;
	ScrollingFcnPlot fc_tempMin;
	ScrollingFcnPlot fc_tempProm;

	// Botones...
	RectButton btnTempActual, btnTempProm, btnTempMin, btnTempMax;
	PFont myFont;

	// Etiquetas textuales...
	boolean alert = false;
	boolean prealert = false;
	Label lbl1, lbl2, lbl3, lbl4, lbl5, lbl6, lbl7, lbl8, lbl9, lbl10, lbl11;
	Label yVal1, yVal2, yVal3;

	// Ventana y viewports...
	int pgFunctionViewportWidth = 480;
	int pgControlViewportWidth = 120;
	int pgViewportsHeight = 240;

	Runnable arduino_running_checker = new Runnable() {
		public void run() {
			while (true) {

				System.out.println("Waiting for the device...");
				while (Serial.list().length == 0)
					;
				openSerial();
				
				// Keep checking if connection
				try {
					while (Serial.list().length != 0) {
						Thread.sleep(100);
					}
				} catch (InterruptedException e) {
				}
				if (port != null) {
					port.clear();
					port.stop();
				}
			}
		}
	};

	public void openSerial() {
		port = new Serial(this, Serial.list()[0], 115200);
		port.clear();
		port.bufferUntil(END_TOKEN);
	}

	public void setup() {

		// Se define el tamaño de la ventana de la aplicación...
		size(pgFunctionViewportWidth + pgControlViewportWidth,
				pgViewportsHeight);

		// Se inicializan los arreglos para almacenar las funciones...
		cantValues = pgFunctionViewportWidth;
		fc_tempActual = new ScrollingFcnPlot(cantValues, color(0, 0, 250), 0,
				100, this);
		fc_tempMax = new ScrollingFcnPlot(cantValues, color(0, 250, 0), 0, 100,
				this);
		fc_tempMin = new ScrollingFcnPlot(cantValues, color(100, 100, 0), 0,
				100, this);
		fc_tempProm = new ScrollingFcnPlot(cantValues, color(250, 0, 0), 0,
				100, this);

		// Se inicializan los botones...
		btnTempActual = new RectButton(pgFunctionViewportWidth + 10, 10, 40,
				40, color(102), color(50), color(255), "MA", this);
		btnTempProm = new RectButton(pgFunctionViewportWidth + 60, 10, 40, 40,
				color(102), color(50), color(255), "MP", this);
		btnTempMin = new RectButton(pgFunctionViewportWidth + 60, 60, 40, 40,
				color(102), color(50), color(255), "MMIN", this);
		btnTempMax = new RectButton(pgFunctionViewportWidth + 10, 60, 40, 40,
				color(102), color(50), color(255), "MMAX", this);

		// Se inicializan los labels...
		lbl1 = new Label(pgFunctionViewportWidth + 10, 110, color(255),
				"Status: ", this);
		lbl2 = new Label(pgFunctionViewportWidth + 60, 110, color(255), "-",
				this);
		lbl3 = new Label(pgFunctionViewportWidth + 10, 130, color(255),
				"T.Actual: ", this);
		lbl4 = new Label(pgFunctionViewportWidth + 70, 130, color(255), "-",
				this);
		lbl5 = new Label(pgFunctionViewportWidth + 10, 150, color(255),
				"T.Max: ", this);
		lbl6 = new Label(pgFunctionViewportWidth + 70, 150, color(255), "-",
				this);
		lbl7 = new Label(pgFunctionViewportWidth + 10, 170, color(255),
				"T.Min: ", this);
		lbl8 = new Label(pgFunctionViewportWidth + 70, 170, color(255), "-",
				this);
		lbl9 = new Label(pgFunctionViewportWidth + 10, 190, color(255),
				"T.Prom: ", this);
		lbl10 = new Label(pgFunctionViewportWidth + 70, 190, color(255), "-",
				this);
		lbl11 = new Label(pgFunctionViewportWidth + 10, 210, color(255, 0, 0),
				"ALERTA!!!", this);

		yVal1 = new Label(10, 5, color(255), "+1", this);
		yVal2 = new Label(10, (pgViewportsHeight - 20) / 2, color(255), "0",
				this);
		yVal3 = new Label(10, pgViewportsHeight - 25, color(255), "-1", this);

		// Inicializa el font de la GUI...
		myFont = createFont("FFScala", 14);
		textFont(myFont);

		// Processing nos pasará por consola la lista de dispositivos USB que
		// están conectados en este momento en el ordenador
		println(Serial.list());

		// Conectamos nuestro objeto puerto al primer dispositivo de la lista
		new Thread(arduino_running_checker).start();

		reading_msg = new byte[MSG_LENGTH];
		writing_msg = new byte[MSG_LENGTH];
	}

	public void draw() {

		// Se actualizan las funciones de ejemplo (f1: coseno(x), f2: función
		// que depende de la posición Y del mouse)
		/*
		 * float amount = map(cosVal, 0, cantValues, 0, 2*PI);
		 * f1.updateValue(cos(amount)); f2.updateValue(height-mouseY);
		 * 
		 * //Se incrementa el ángulo de la función coseno... cosVal = (cosVal +
		 * 1) % cantValues;
		 */
		// Se actualizan las funciones de cada temperatura y labels
		fc_tempActual.updateValue((float) tempActual);
		fc_tempMax.updateValue((float) tempMax);
		fc_tempMin.updateValue((float) tempMin);
		fc_tempProm.updateValue((float) tempProm);
		lbl4.writeTemp(tempActual);
		lbl6.writeTemp(tempMax);
		lbl8.writeTemp(tempMin);
		lbl10.writeTemp(tempProm);

		// Se actualiza el label del modo actual
		switch (mode) {
		case MODO_TEMP_ACTUAL:
			lbl2.caption = "ACT";
			break;
		case MODO_TEMP_MAXIMA:
			lbl2.caption = "MAX";
			break;
		case MODO_TEMP_MINIMA:
			lbl2.caption = "MIN";
			break;
		case MODO_TEMP_PROMEDIO:
			lbl2.caption = "PROM";
			break;
		}
		// ReadMessage();
		// Rendering de la interface...
		background(125);
		stroke(0);
		noFill();

		// Dibuja las funciones...
		fc_tempActual.displayIntoRect(30, 10, pgFunctionViewportWidth - 10,
				pgViewportsHeight - 10);
		fc_tempMax.displayIntoRect(30, 10, pgFunctionViewportWidth - 10,
				pgViewportsHeight - 10);
		fc_tempMin.displayIntoRect(30, 10, pgFunctionViewportWidth - 10,
				pgViewportsHeight - 10);
		fc_tempProm.displayIntoRect(30, 10, pgFunctionViewportWidth - 10,
				pgViewportsHeight - 10);

		// Procesa eventos de MouseOver...
		btnTempActual.update();
		btnTempProm.update();
		btnTempMin.update();
		btnTempMax.update();

		// Procesa las entradas (botones)
		if (mousePressed) {
			// println("presione boton");
			if (btnTempActual.pressed()) {
				ultimo = MODO_TEMP_ACTUAL;
				btnTempActual.currentcolor = color(0, 100, 0);
				lbl2.caption = "MA";

				prealert = true;
			} else if (btnTempProm.pressed()) {
				ultimo = MODO_TEMP_PROMEDIO;
				btnTempProm.currentcolor = color(0, 100, 0);
				lbl2.caption = "MP";
				// mode = MODO_TEMP_PROMEDIO;
				// sendMessage();
				prealert = true;
			} else if (btnTempMin.pressed()) {
				ultimo = MODO_TEMP_MINIMA;
				btnTempMin.currentcolor = color(0, 100, 0);
				lbl2.caption = "MMIN";
				// mode = MODO_TEMP_MINIMA;
				// sendMessage();
				prealert = true;
			} else if (btnTempMax.pressed()) {
				ultimo = MODO_TEMP_MAXIMA;
				btnTempMax.currentcolor = color(0, 100, 0);
				lbl2.caption = "MMAX";
				// mode = MODO_TEMP_MAXIMA;
				// sendMessage();
				prealert = true;
			}
		}

		// Dibuja el eje X y el recuadro de los gráficos...
		stroke(0);
		line(30, pgViewportsHeight / 2, pgFunctionViewportWidth - 10,
				pgViewportsHeight / 2);
		rect(30, 10, pgFunctionViewportWidth - 40, pgViewportsHeight - 20);

		// Se dibujan los botones...
		btnTempActual.display();
		btnTempProm.display();
		btnTempMin.display();
		btnTempMax.display();

		// Se dibuja texto adicional...(labels, etc)
		lbl1.display();
		lbl2.display();
		lbl3.display();
		lbl4.display();
		lbl5.display();
		lbl6.display();
		lbl7.display();
		lbl8.display();
		lbl9.display();
		lbl10.display();
		/*
		 * switch(mode){ case MODE_TEMP_ACTUAL: lbl3.display(); lbl4.display();
		 * break; case MODE_TEMP_MAX: lbl5.display(); lbl6.display(); break;
		 * case MODE_TEMP_MIM: lbl7.display(); lbl8.display(); break; case
		 * MODE_TEMP_PROM: lbl9.display(); lbl4.display(); break;
		 */

		// if (alert) lbl11.display();
		if (tempActual > 60)
			lbl11.display();

		yVal1.display();
		yVal2.display();
		yVal3.display();
	}

	void ParseMessage() {
		// Parse mode
		mode = (reading_msg[0] << 8) | reading_msg[1]; // println("mode "
														// +mode);

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
		// println("temp acutal " +temps[0]);

	}

	void ReadMessage() {
		// Leemos mensaje
		int msg_pointer = 0;
		while (true) {
			if (port.available() == 0)
				continue;
			current_byte = (byte) port.read();
			if (current_byte == START_TOKEN) {
				msg_pointer = 0;
				print("start ");
				continue;
			}
			// End token read
			if (current_byte == END_TOKEN) {
				// REMOVED: Additional check, is pointer at the end?
				println("end");
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

			print(current_byte + " ");
		}
	}

	void sendMessage() {
		double[] temps = new double[4];

		// Put temps in array
		temps[0] = tempActual;
		temps[1] = tempMax;
		temps[2] = tempMin;
		temps[3] = tempProm;

		// Write mode
		writing_msg[0] = (byte) (mode >> 8);
		writing_msg[1] = (byte) (mode);

		for (int i = 1; i < 5; i++) {
			writing_msg[2 * i] = (byte) (temps[i - 1] + NUMBER_PADDING);
			writing_msg[2 * i + 1] = (byte) ((byte) (temps[i - 1] * 100) % 100 + NUMBER_PADDING);
		}

		// Write start token
		port.write(START_TOKEN);

		// Write message
		for (int i = 0; i < MSG_LENGTH; i++)
			port.write(writing_msg[i]);

		// Write end token
		port.write(END_TOKEN);
	}

	public void mouseReleased() {
		// Si se pulsó algún botón y se completa el click, se hace el toggle
		// sobre la etiqueta de alerta...
		if (prealert) {
			alert = !alert;
			prealert = false;
		}
	}

	public void mouseClicked() {
		mode = ultimo;
		sendMessage();
	}

	public void serialEvent(Serial p) {
		ReadMessage();
	}

}
