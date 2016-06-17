#ifndef VEC3_H_
#define VEC3_H_
#include <cmath>

#ifndef VEC2_H_
class Vec2;
#endif

#ifndef QUAT_H_
class Quat;
#endif

#ifndef VEC4_H_
class Vec4;
#endif


/**
 * \class Vec3
 * \brief 3 component vector (x,y,z)
 * \author Joseph Cutler
 * \date
 */
class Vec3{
public:
	/** x component of Vec3 */
	float x; 
	 /** y component of Vec3 */
	float y;
	 /** z component of Vec3 */
	float z;

	Vec3();
	Vec3(float,float,float);

	/**  Unit vector in +x direction */
	static const Vec3 I;	
	/**  Unit vector in +y direction */
	static const Vec3 J;	
	/**  Unit vector in +z direction */
	static const Vec3 K;	

	///Magnitude (or size) of Vec3.
	float mag() const;

	///Sum of this and v
	Vec3 operator+(const Vec3& v) const;
	///Difference of this and v
	Vec3 operator-(const Vec3& v) const;
	///Scalar multiply this by f
	Vec3 operator*(float f) const;
	///Scalar divide this by f
	Vec3 operator/(float f) const;
	///The negative of this
	Vec3 operator-() const;
	bool operator==(const Vec3 &v) const;

	/**
	 * @brief Cast Vec3 to a Quat
	 * @note w component of the Quat will be initialized to 0
	 */
	operator Quat() const;
	///@brief Cast Vec3 to a Vec2 
	operator Vec2() const;
	/**
	 * @brief Cast Vec3 to a Vec4
	 * @note w component of Vec4 will be initialized to 1
	 */
	operator Vec4() const;

	Vec3 normalized() const;
	void normalizeInPlace();
	static void normalize(Vec3& v);

	float dot(const Vec3& v) const;
	static float dot(const Vec3& a,const Vec3& b);

	float squared() const;

	Vec3 cross(const Vec3& v) const;
	void crossInPlace(const Vec3& v);

	///Return this rotated by Quat q
	Vec3 rotate(const Quat& q) const;

};

#endif
