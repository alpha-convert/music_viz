/*
 * Color.h
 *
 *  Created on: Oct 28, 2015
 *      Author: Admin
 */

#ifndef COLOR_COLOR_H_
#define COLOR_COLOR_H_
#include <stdint.h>
#include <algorithm>
#include "stdint.h"
#include "macros.h"

class Color {
public:
	float r;
	float g;
	float b;
	float a;
	Color();
	Color(float r, float g, float b);
	Color(float r, float g, float b, float a);
	virtual ~Color(){};

	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Black;
	static const Color White;
	static const Color Clear;

	bool operator==(const Color& c) const;
	Color operator*(float f) const;
	Color operator/(float f) const;
	Color operator+(const Color &c) const;

	Color UnclampedAdd(const Color &c) const;
	Color UnclampedMult(float f) const;

	void clamp();
	Color clamped();

};


#endif /* COLOR_COLOR_H_ */
