#include "Ray.h"
Ray::Ray(){
	orig = Vec3(0,0,0);
	dir = Vec3(0,0,0);
}

Ray::Ray(Vec3 orig, Vec3 dir) : orig(orig), dir(dir){}


//TODO: Make this actually work
Ray Ray::ThroughPixel(float x, float y, const Vec3 &camera_pos, const Vec3& screen_top_left){
	Ray cast_ray;
	cast_ray.orig = camera_pos;

	auto px_vec = camera_pos * SGN(camera_pos.z) + screen_top_left;
	px_vec.x += x;
	px_vec.y -= y;
	//Set and normalize the ray direction
	cast_ray.dir = px_vec.normalized();
	
	return cast_ray;
}

Vec3 Ray::Along(float dist) const{
	return orig + dir * dist;
}

