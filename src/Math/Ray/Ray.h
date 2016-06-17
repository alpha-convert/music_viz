#pragma once
/***
* @Author Joseph Cutler
* @Date April 07, 2016
* @Copyright WTFPL
*/
#include "Math/Vec3/Vec3.h"
#include "macros.h"
class Ray {
public:
	Vec3 orig;
	Vec3 dir;

	static Ray ThroughPixel(float x, float y, const Vec3 &camera_pos, const Vec3& screen_top_left);
	Vec3 Along(float dist) const;
	Ray();
	Ray(Vec3 orig, Vec3 dir);
	virtual ~Ray(){};
private:
};
