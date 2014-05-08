package lab4processing;

import processing.core.PApplet;

/**
 * Clases que definen botones. Se basan en el ejemplo Topics > GUI > Buttons de
 * Processing 1.5.
 * 
 * http://processing.org/learning/topics/buttons.html
 * 
 * Modificaciones introducidas por: Sebastián Escarza Sistemas Embebidos - 1ºC
 * de 2012.
 */

class Button {
	PApplet parent;
	int x, y;
	int size, size2;
	int basecolor, highlightcolor;
	int currentcolor, textcolor;
	boolean over = false;
	boolean pressed = false;
	boolean locked = false;
	String caption = "Button";

	Button(PApplet parent) {
		this.parent = parent;
	}

	void update() {
		if (over()) {
			currentcolor = highlightcolor;
		} else {
			currentcolor = basecolor;
		}
	}

	boolean pressed() {
		if (over) {
			locked = true;
			return true;
		} else {
			locked = false;
			return false;
		}
	}

	boolean over() {
		return true;
	}

	boolean overRect(int x, int y, int width, int height) {
		if (parent.mouseX >= x && parent.mouseX <= x + width
				&& parent.mouseY >= y && parent.mouseY <= y + height) {
			return true;
		} else {
			return false;
		}
	}

	boolean overCircle(int x, int y, int diameter) {
		float disX = x - parent.mouseX;
		float disY = y - parent.mouseY;
		if (PApplet.sqrt(PApplet.sq(disX) + PApplet.sq(disY)) < diameter / 2) {
			return true;
		} else {
			return false;
		}
	}

}

class CircleButton extends Button {
	CircleButton(int ix, int iy, int isize, int icolor, int ihighlight,
			int textcolor, String caption, PApplet parent) {
		super(parent);
		x = ix;
		y = iy;
		size = isize;
		basecolor = icolor;
		highlightcolor = ihighlight;
		currentcolor = basecolor;
		this.textcolor = textcolor;
		this.caption = caption;
	}

	boolean over() {
		if (overCircle(x, y, size)) {
			over = true;
			return true;
		} else {
			over = false;
			return false;
		}
	}

	void display() {
		parent.stroke(255);
		parent.fill(currentcolor);
		parent.ellipse(x, y, size, size);
		parent.fill(textcolor);
		parent.textAlign(PApplet.CENTER, PApplet.CENTER);
		parent.text(caption, x + size / 2, y + size / 2);
	}
}

class RectButton extends Button {
	RectButton(int ix, int iy, int isize, int isize2, int icolor,
			int ihighlight, int textcolor, String caption, PApplet parent) {
		super(parent);
		x = ix;
		y = iy;
		size = isize;
		size2 = isize2;
		basecolor = icolor;
		highlightcolor = ihighlight;
		currentcolor = basecolor;
		this.textcolor = textcolor;
		this.caption = caption;
	}

	boolean over() {
		if (overRect(x, y, size, size2)) {
			over = true;
			return true;
		} else {
			over = false;
			return false;
		}
	}

	void display() {
		parent.stroke(255);
		parent.fill(currentcolor);
		parent.rect(x, y, size, size2);
		parent.fill(textcolor);
		parent.textAlign(PApplet.CENTER, PApplet.CENTER);
		parent.text(caption, x + size / 2, y + size2 / 2);
	}
}