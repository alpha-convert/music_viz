#include "macros.h"
#include "Vec4.h"
#include "Vec3/Vec3.h"
#include "Vec3/Vec3.h"
#include <cmath>
#include <assert.h>
Vec4::Vec4() : x(0), y(0), z(0), w(1) {};

Vec4::Vec4(float x,float y,float z,float w) : x(x), y(y), z(z), w(w) {};
Vec4::Vec4(float x,float y,float z) : x(x), y(y), z(z), w(1) {};

const Vec4 Vec4::I = Vec4(1,0,0,1);
const Vec4 Vec4::J = Vec4(0,1,0,1);
const Vec4 Vec4::K = Vec4(0,0,1,1);
const Vec4 Vec4::L = Vec4(0,0,0,1);

float Vec4::mag() const{
	return sqrt(SQ(x) + SQ(y) + SQ(z));
}

Vec4 Vec4::operator+(const Vec4& v) const{
	return Vec4(x+v.x,
				y+v.y,
				z+v.z,
				w+v.w);
}

Vec4 Vec4::operator-(const Vec4& v) const{
	return Vec4(x-v.x,
				y-v.y,
				z-v.z,
				w-v.w);
}


Vec4 Vec4::operator*(float f) const{
	return Vec4(x*f,y*f,z*f,w*f);
}
Vec4 Vec4::operator/(float f) const{
	return *this * (1/f);
}

Vec4::operator Vec3() const{
	Vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

Vec4 Vec4::normalized() const{
	return *this/mag();
}

void Vec4::normalize(Vec4& v){
	v = v.normalized();
}

float Vec4::dot(const Vec4& v) const{
	return v.x*x + v.y*y + v.z*z + v.w*w;
}

float Vec4::dot(const Vec4& a, const Vec4& b){
	return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

Vec4 Vec4::cross(const Vec4& v) const{
	return (Vec4) (((Vec3) v).cross((Vec3) *this));
}


Vec4 Vec4::rotate(const Quat& q) const{
	auto as_vec3 = Vec3(x,y,z);
	auto rotated = as_vec3.rotate(q);
	
	return Vec4(rotated.x,rotated.y,rotated.z);
}
