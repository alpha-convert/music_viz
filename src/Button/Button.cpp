#include "Button.h"

Button::~Button(){}
Button::Button():code(0){};
Button::Button(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char code): x(x), y(y), width(w), height(h), code(code){
	on_text = "Play";
	off_text = "Pause";
	on_c = Color::Red;
	off_c = Color::Green;
	border_color = Color::Black;
	state = Button::Up;
}

bool Button::mouseInside(unsigned int mx, unsigned int my) const{
	auto inx = mx > x && mx < (mx + width);
	auto iny = my > y && my < (my + height);
	return inx && iny;
}

void Button::toggleState(){
	state = !state;
}

bool Button::getState() const{
	return state;
}

char Button::getCode() const{
	return code;
}

unsigned int Button::getHeight() const{
	return height;
}

unsigned int Button::getWidth() const{
	return width;
}

unsigned int Button::getX() const{
	return x;
}

unsigned int Button::getY() const{
		return y;
}

std::string Button::getText() const{
	return state ? on_text : off_text;
}

Color Button::getColor() const{
	return state ? on_c : off_c;
}

