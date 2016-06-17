/*
 * Color.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: Admin
 */

#include "Color.h"
Color::Color(){
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->a = 1;
	clamp();
}
Color::Color(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = 1;
	clamp();
}

Color::Color(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

const Color Color::Red 		= Color(1,0,0);
const Color Color::Green	= Color(0,1,0);
const Color Color::Blue		= Color(0,0,1);
const Color Color::Black	= Color(0,0,0);
const Color Color::White	= Color(1,1,1);
const Color Color::Clear 	= Color(0,0,0);

bool Color::operator==(const Color& c) const{
	return (r == c.r) && (g == c.g) && (b == c.b) && (a == c.a); 
}

Color Color::operator*(float f) const{
	return Color(f*r,f*g,f*b,a).clamped();
}
Color Color::operator/(float f) const{
	return Color(r/f,g/f,b/f,a).clamped();
}

Color Color::operator+(const Color &c) const{
	return Color(r+c.r,g+c.g,b+c.b,a).clamped();
}

Color Color::UnclampedAdd(const Color &c) const{
	return Color(r+c.r,g+c.g,b+c.b,a);
}

Color Color::UnclampedMult(float f) const{
	return Color(f*r,f*g,f*b,a);
}

Color Color::clamped(){
	auto c = *this;
	c.clamp();
	return c;
}

void Color::clamp(){
	r = CLAMP(r,0.0,1.0);
	g = CLAMP(g,0.0,1.0);
	b = CLAMP(b,0.0,1.0);
	a = CLAMP(a,0.0,1.0);
}
