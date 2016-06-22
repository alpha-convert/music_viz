#include "Button.h"

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

bool Button::mouseInside(unsigned int mx, unsigned int my) const{
	auto inx = mx > x && mx < (mx + width);
	auto iny = my > y && my < (my + height);
	return inx && iny;
}

