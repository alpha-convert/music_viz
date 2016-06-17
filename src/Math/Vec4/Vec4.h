#ifndef VEC4_H_
#define VEC4_H_ 

#ifndef VEC3_H_
class Vec3;
#endif

#ifndef QUAT_H_
class Quat;
#endif

/**
 * \class Vec4
 * \brief 4 component vector (x,y,z,w)
 * \author Joseph Cutler
 * \date
 */

class Vec4{
public:
	float x; ///<X component of Vec4
	float y; ///<Y component of Vec4
	float z; ///<Z component of Vec4
	float w; ///<W component of Vec4

	///Default Constructor
	/**
	 * Default constructor for Vec4. Initializes all fields to 0 but W component, which starts as 1.
	 */
	Vec4(); 

	///4 Component constructor
	Vec4(float,float,float,float);

	///3 Component constructor
	/**
	 * This constructor initializes W to 1.
	 */
	Vec4(float,float,float);

	///Unit vector in +x direction
	static const Vec4 I;
	///Unit vector in +y direction
	static const Vec4 J;
	///Unit vector in +z direction
	static const Vec4 K;
	///Unit vector in +w direction
	/**
	 * Zero vector in R3
	 */
	static const Vec4 L;

	///Magnitude (or size) of Vec4.
	float mag() const;


	///Sum of this and v
	Vec4 operator+(const Vec4& v) const;
	///Difference of this and v
	Vec4 operator-(const Vec4& v) const;
	///Scalar multiply this by f
	Vec4 operator*(float f) const;
	///Scalar divide this by f
	Vec4 operator/(float f) const;

	///Cast Vec4 to Vec3
	/**
	 * @warning Casting from Vec4 to Vec3 loses w component info
	 */
	operator Vec3() const;

	///Returns a normalized version of this
	Vec4 normalized() const;
	static void normalize(Vec4& v);

	///Dot product (scalar product) of v and this
	float dot(const Vec4& v) const;

	///Dot product (scalar product) of a and b
	static float dot(const Vec4& a, const Vec4& b);

	///Cross product (Vector product) of v and this
	/**
	 * @warning The cross product does not commute. AxB != BxA
	 */
	Vec4 cross(const Vec4& v) const;


	///Returns a rotated version of this by q.
	/**
	 * @warning Q must be a Unit Quaternion for the desired result
	 */
	Vec4 rotate(const Quat& q) const;

};
#endif
