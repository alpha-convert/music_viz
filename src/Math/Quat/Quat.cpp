#include <cmath>
#include "Vec2/Vec2.h"
#include "Vec3/Vec3.h"
#include "Quat.h"

Quat::Quat(){
	x = y = z = w = 0;
}

Quat::Quat(Vec3& v, float w){
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = w;
}


Quat Quat::rotation(const Vec3& axis, float theta){
	Quat q;
	q.w = cos(theta/2);
	q.x = axis.x * sin(theta/2);
	q.y = axis.y * sin(theta/2);
	q.z = axis.z * sin(theta/2);
	return q;
}



Vec3 Quat::vecPart(){
	return Vec3(x,y,z);
}

Quat Quat::operator+(const Quat& q) const{
	Quat q2;
	q2.x = x + q.x;
	q2.y = y + q.y;
	q2.z = z + q.z;
	q2.w = w + q.w;
	return q2;
}


Quat Quat::operator-(const Quat& q) const{
	Quat q2;
	q2.x = x - q.x;
	q2.y = y - q.y;
	q2.z = z - q.z;
	q2.w = w - q.w;
	return q2;
}


Quat Quat::operator*(float f) const{
	return Quat(x*f,y*f,z*f,w*f);
}

Quat Quat::operator/(float f) const{
	return Quat(x/f,y/f,z/f,w/f);
}

Quat Quat::operator*(const Quat& q) const{
	Quat product;
	auto a1 = w;
	auto b1 = x;
	auto c1 = y;
	auto d1 = z;

	auto a2 = q.w;
	auto b2 = q.x;
	auto c2 = q.y;
	auto d2 = q.z;

	product.w = a1*a2 - b1*b2 - c1*c2 - d1*d2;
	product.x = a1*b2 + b1*a2 + c1*d2 - d1*c2;
	product.y = a1*c2 - b1*d2 + c1*a2 + d1*b2;
	product.z = a1*d2 + b1*c2 - c1*b2 + d1*a2;

	return product;	

}

Quat::operator Vec3() const{
	return Vec3(x,y,z);
}


#define SQ(x) (x*x)
float Quat::mag() const{
	return sqrt(SQ(x) + SQ(y) + SQ(z) + SQ(w));
}
#undef SQ

Quat Quat::conj() const{
	Quat q;
	q.x = -x;
	q.y = -y;
	q.z = -z;
	q.w = w;
	return q;
}

Quat Quat::conj(const Quat& q){
	return q.conj();
}

void Quat::conjInPlace(){
	*this = this->conj();
}


Quat Quat::normalized() const{
	return (*this / mag());
}

Quat Quat::normalize(const Quat& q){
	return (q / q.mag());
}

void Quat::normalizeInPlace(){
	auto v = this->normalized();
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}
