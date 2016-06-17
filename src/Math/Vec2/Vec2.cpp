#include "Vec2.h"
#include "Vec3/Vec3.h"
#include <cmath>

Vec2::Vec2(float x,float y) : x(x), y(y){}

Vec2::Vec2() : x(0), y(0) {}

const Vec2 Vec2::I = Vec2(1,0);
const Vec2 Vec2::J = Vec2(0,1);

#define SQ(x) (x*x)
float Vec2::mag() const{
	return sqrt(SQ(this->x) + SQ(this->y));

}
#undef SQ

float Vec2::angle() const{
	return atan2(this->y,this->x);
}

Vec2 Vec2::operator+(const Vec2& v) const{
	float nX = v.x + x;
	float nY = v.y + y;
	return Vec2(nX,nY);
}

Vec2 Vec2::operator-(const Vec2& v) const{
	float nX = x - v.x;
	float nY = y - v.y;
	return Vec2(nX,nY);
}

Vec2 Vec2::operator*(float f) const{
	return Vec2(x*f,y*f);
}

Vec2 Vec2::operator/(float f) const{
	return Vec2(x/f,y/f);
}


Vec2::operator Vec3() const{
	return Vec3(x,y,0);
}


Vec2 Vec2::normalized() const{
	auto nX = x / mag();
	auto nY = y / mag();
	return Vec2(nX,nY);
}
void Vec2::normalizeInPlace(){
	*this = this->normalized();
}

void Vec2::normalize(Vec2& v){
	v = v.normalized();
}


float Vec2::dot(const Vec2& v) const{
	return x*v.x + y*v.y;
}
float Vec2::dot(const Vec2& a,const Vec2& b){
	return a.dot(b);
}

Vec3 Vec2::cross(const Vec2& v) const{
	Vec3 c;
	c.x = y*0 - 0*v.y;
	c.y = 0*v.x - x*0;
	c.z = x*v.y - y*v.x;
	return c;

}
Vec3 Vec2::cross(const Vec3& v) const{
	Vec3 c;
	c.x = y*v.z;
	c.y = -x*v.z;
	c.z = x*v.y - y*v.x;
	return c;
}

