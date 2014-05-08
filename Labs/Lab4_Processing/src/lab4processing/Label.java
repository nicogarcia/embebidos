package lab4processing;

import processing.core.PApplet;

/**
 * Clase que define etiquetas textuales para la GUI de la aplicación. Define una
 * posición (esquina superior izquierda) para la etiqueta, el contenido de la
 * misma y el color para el renderizado del texto.
 * 
 * Autor: Sebastián Escarza Sistemas Embebidos - 1ºC de 2012.
 */

class Label {
	int x, y;
	int textcolor;
	String caption;
	PApplet parent;

	Label(int x, int y, int textcolor, String caption, PApplet p) {
		this.parent = p;
		this.x = x;
		this.y = y;
		this.textcolor = textcolor;
		this.caption = caption;
	}

	void display() {
		parent.stroke(255);
		parent.fill(textcolor);
		parent.textAlign(PApplet.LEFT, PApplet.TOP);
		parent.text(caption, x, y);
	}

	void writeTemp(double temp) {
		this.caption = "" + (int) temp + "." + (int) (temp * 10) % 10 + "°";
	}
}