#ifndef VEC2_H_
#define VEC2_H_ 

#ifndef VEC3_H_
class Vec3;
#endif

#ifndef QUAT_H_ 
class Quat;
#endif

/***
* @Author Joseph Cutler
* @Copyright WTFPL
*/
class Vec2{
public:
	/** x component of Vec2*/
	float x;
	/** y component of Vec2*/
	float y;

	Vec2();
	Vec2(float,float);

	static const Vec2 I;
	static const Vec2 J;

	float mag() const;
	float angle() const;

	Vec2 operator+(const Vec2& v) const;
	Vec2 operator-(const Vec2& v) const;
	Vec2 operator*(float f) const;
	Vec2 operator/(float f) const;

	operator Vec3() const;



	Vec2 normalized() const;
	void normalizeInPlace();
	static void normalize(Vec2& v);

	//the dot product.
	float dot(const Vec2& v) const;
	static float dot(const Vec2& a,const Vec2& b);

	//Cross product: Inherently 3d operation
	Vec3 cross(const Vec2& v) const;
	Vec3 cross(const Vec3& v) const;

};

#endif
