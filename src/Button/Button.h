#pragma once
#include <functional>
#include <string>
#include "Color.h"

/***
* @Author Joseph Cutler
* @Date June 22, 2016
* @Copyright WTFPL
*/


class Button {
public:

	static constexpr bool Down = true;
	static constexpr bool Up = false;

	Button(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char code);
	~Button();
	Button();

	bool mouseInside(unsigned int mx, unsigned int my) const;

	void setOnState(const std::string &s, const Color &c);

	void toggleState();

	bool getState() const;
	char getCode() const;
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	unsigned int getX() const;
	unsigned int getY() const;
	std::string getText() const;
	Color getColor() const;
private:
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	char code;

	bool state;

	std::string on_text;
	std::string off_text;
	Color on_c;
	Color off_c;
	Color border_color;

};
