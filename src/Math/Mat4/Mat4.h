#ifndef MAT4_H_
#define MAT4_H_ 

#include <cmath>
#include <vector>
#include <algorithm>

#ifndef VEC3_H_
class Vec3;
#endif

#ifndef VEC4_H_
class Vec4;
#endif

class Mat4{
public:
	Mat4();
	Mat4(std::vector<float> data);
	virtual ~Mat4(){};

	void print();

    float& operator[](unsigned index);
    float operator[](unsigned index) const;
	static Mat4 Ident();
	static Mat4 Projection();

	static Mat4 Translation(float x, float y, float z);
	static Mat4 Translation(const Vec3& v);
	static Mat4 Translation(const Vec4& v);

	static Mat4 Scale(float f);

	Mat4 operator*(float f) const;
	Mat4 operator*(const Mat4& m) const;
	Vec4 operator*(const Vec4& v) const;


	//TODO: This is nice!
	//http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/jay.htm



protected:
	std::vector<float> data;
};
#endif
