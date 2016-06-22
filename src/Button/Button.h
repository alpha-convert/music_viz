#pragma once
#include <functional>
#include "Visualizer.h"

/***
* @Author Joseph Cutler
* @Date June 22, 2016
* @Copyright WTFPL
*/

class Button {
public:
	Button(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	~Button();

	bool mouseInside(unsigned int mx, unsigned int my) const;

	unsigned int getHeight() const;
	unsigned int getWidth() const;
	unsigned int getX() const;
	unsigned int getY() const;
private:
	unsigned int height;
	unsigned int width;
	unsigned int x;
	unsigned int y;

	std::string text;
	Color c;


};
