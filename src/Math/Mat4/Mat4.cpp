#include "Mat4.h"
#include "Vec3/Vec3.h"
#include "Vec4/Vec4.h"
Mat4::Mat4() : data(16){
	std::fill(data.begin(), data.end(), float(0));
	data[0] = data[5] = data[10] = data[15] = 1.0f;
}

Mat4::Mat4(std::vector<float> data) : data(data){};

Mat4 Mat4::Ident(){
	Mat4 m;
	std::fill(m.data.begin(), m.data.end(), static_cast<float>(0));
	m.data[0] = m.data[5] = m.data[10] = m.data[15] = 1.0f;
	return m;
}

Mat4 Mat4::Translation(float x, float y, float z){
	Mat4 m = Mat4::Ident();
	m.data[3] = x;
	m.data[7] = y;
	m.data[11] = z;
	return m;
}


Mat4 Mat4::Translation(const Vec3& v){
	return Mat4::Translation(v.x,v.y,v.z);
}
Mat4 Mat4::Translation(const Vec4& v){
	return Mat4::Translation(v.x,v.y,v.z);
}

Mat4 Mat4::Scale(float f){
	Mat4 m = Mat4::Ident();
	m.data[0] = m.data[5] = m.data[10] = f;
	return m;
}

Mat4 Mat4::Projection(){
	Mat4 m = Mat4::Ident();
	m[15] = 0;
	m[14] = 1;
	return m;
}

float& Mat4::operator[](unsigned index){
	if(index >= 16){
		std::out_of_range e("Mat4 access out of range");
		throw e;
	}
	return data[index];
}

float Mat4::operator[](unsigned index) const{
	if(index >= 16){
		std::out_of_range e("Mat4 access out of range");
		throw e;
	}
	return data[index];
}

Vec4 Mat4::operator*(const Vec4& v) const{
	Vec4 c;
	auto vx = v.x;
	auto vy = v.y;
	auto vz = v.z;
	auto vw = v.w;
	c.x = vx*data[0] + vy*data[1] + vz*data[2] + vw*data[3];
	c.y = vx*data[4] + vy*data[5] + vz*data[6] + vw*data[7];
	c.z = vx*data[8] + vy*data[9] + vz*data[10] + vw*data[11];
	c.w = vx*data[12] + vy*data[13] + vz*data[14] + vw*data[15];

	return c;
}

Mat4 Mat4::operator*(const Mat4& b) const{
	auto a = *this;
	//who cares, i'm unrolling this.
	Mat4 c = Ident();
	c[0] = a[0]*b[0] + a[1]*b[4] + a[2]*b[8] + a[3]*b[12];
	c[1] = a[0]*b[1] + a[1]*b[5] + a[2]*b[9] + a[3]*b[13];
	c[2] = a[0]*b[2] + a[1]*b[6] + a[2]*b[10] + a[3]*b[14];
	c[3] = a[0]*b[3] + a[1]*b[7] + a[2]*b[11] + a[3]*b[15];

	c[4] = a[4]*b[0] + a[5]*b[4] + a[6]*b[8] + a[7]*b[12];
	c[5] = a[4]*b[1] + a[5]*b[5] + a[6]*b[9] + a[7]*b[13];
	c[6] = a[4]*b[2] + a[5]*b[6] + a[6]*b[10] + a[7]*b[14];
	c[7] = a[4]*b[3] + a[5]*b[7] + a[6]*b[11] + a[7]*b[15];

	c[8] = a[8]*b[0] + a[9]*b[4] + a[10]*b[8] + a[11]*b[12];
	c[9] = a[8]*b[1] + a[9]*b[5] + a[10]*b[9] + a[11]*b[13];
	c[10] = a[8]*b[2] + a[9]*b[6] + a[10]*b[10] + a[11]*b[14];
	c[11] = a[8]*b[3] + a[9]*b[7] + a[10]*b[11] + a[11]*b[15];

	c[12] = a[12]*b[0] + a[13]*b[4] + a[14]*b[8] + a[15]*b[12];
	c[13] = a[12]*b[1] + a[13]*b[5] + a[14]*b[9] + a[15]*b[13];
	c[14] = a[12]*b[2] + a[13]*b[6] + a[14]*b[10] + a[15]*b[14];
	c[15] = a[12]*b[3] + a[13]*b[7] + a[14]*b[11] + a[15]*b[15];
	return c;
}

void Mat4::print(){
	printf("%.2f  %.2f  %.2f  %.2f\n",data[0], data[1], data[2], data[3]);
	printf("%.2f  %.2f  %.2f  %.2f\n",data[4], data[5], data[6], data[7]);
	printf("%.2f  %.2f  %.2f  %.2f\n",data[8], data[9], data[10], data[11]);
	printf("%.2f  %.2f  %.2f  %.2f\n",data[12], data[13], data[14], data[15]);
}
