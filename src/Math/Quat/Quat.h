#ifndef QUAT_H_
#define QUAT_H_ 

#ifndef VEC3_H_
class Vec3;
#endif
#ifndef VEC2_H_
class Vec2;
#endif

class Quat{
public:
	float x,y,z,w;
	Quat();
	Quat(float x,float y,float z,float w) : x(x), y(y), z(z), w(w){};
	Quat(Vec3& v, float w);

	static Quat rotation(const Vec3& axis, float theta);


	Quat operator+(const Quat& q) const;
	Quat operator-(const Quat& q) const;
	Quat operator*(float f) const;
	Quat operator/(float f) const;
	Quat operator-() const;

	Quat operator*(const Quat& q) const;

	operator Vec3() const;

	float mag() const;

	Quat conj() const;
	static Quat conj(const Quat& q);
	void conjInPlace();

	Quat normalized() const;
	static Quat normalize(const Quat& q);
	void normalizeInPlace();


	virtual ~Quat(){};

private:
	Vec3 vecPart();

};
#endif
