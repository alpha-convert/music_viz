/*
 * Graphics.cpp
 *
 *  Created on: Oct 28, 2015
 *      Author: Admin
 */
#include "Graphics.h"
#include <SDL2/SDL.h>
#include <execinfo.h>

#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include <array>
#include <functional>
#include <algorithm>

//General graphics flow:

Graphics::Graphics(uint32_t width, uint32_t height, const char *name) {
	if (!SDL_WasInit(SDL_INIT_VIDEO)) {
		SDL_Init(SDL_INIT_VIDEO);
	}
	this->width = width;
	this->height = height;

	this->window = SDL_CreateWindow(name,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
			SDL_WINDOW_SHOWN);
	if (!this->window) {
		fprintf(stderr, "Error creating window, %s\n", SDL_GetError());
	}
	assert(window != nullptr);

	this->renderer = SDL_CreateRenderer(this->window, -1,
			SDL_RENDERER_ACCELERATED);
	if (!this->renderer) {
		fprintf(stderr,"Error creating renderer: %s", SDL_GetError());
	}
	assert(renderer!= nullptr);


	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);


}

void Graphics::Clear() {
	SetColor(Color::White);
	if(renderer != nullptr){
		SDL_RenderClear(this->renderer);
	}
}

void Graphics::Update() {
	for(const auto &p : points_to_draw){
		SetColor(p.c);
		SDL_RenderDrawPoint(renderer,p.p.x,p.p.y);
	}
	for(const auto &r : rects_to_draw){
		SetColor(r.c);
		SDL_RenderDrawRect(renderer,&r.r);
	}
	for(const auto &r : filled_rects_to_draw){
		SetColor(r.c);
		SDL_RenderFillRect(renderer,&r.r);
	}
	for(const auto &l : lines_to_draw){
		SetColor(l.c);
		SDL_RenderDrawLine(renderer,l.x1,l.y1,l.x2,l.y2);
	}
	SDL_UpdateWindowSurface(this->window);
	SDL_RenderPresent(this->renderer);

	points_to_draw.clear();
	rects_to_draw.clear();
	filled_rects_to_draw.clear();
	lines_to_draw.clear();
}

void Graphics::ProjectVec3(const Vec3 &v, const Color &c, int scalar){
	this->SpaceLine(0, 0, v.dot(Vec3::I) * scalar, v.dot(Vec3::J) * scalar, c);
}

void Graphics::LineFromVec(const Vec3 &v1, const Vec3 &v2, const Color &c){
	this->SpaceLine(v1.x, v1.y, v2.x, v2.y, c);
}

void Graphics::SpaceLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2,const Color &c){
	Line(this->width / 2 + x1, this->height / 2 - y1, this->width / 2 + x2,
			this->height / 2 - y2, c);
}

void Graphics::Line(int32_t x1, int y1, int x2, int y2, const Color &c){
	DeferredRenderLine r;
	r.c = c;
	r.x1 = x1;
	r.y1 = y1;
	r.x2 = x2;
	r.y2 = y2;
	lines_to_draw.push_back(r);
}

void Graphics::PutPixel(int32_t x, int32_t y, const Color &c) {
	if (!(x > this->width && x < 0 && y > this->height && y < 0)) {
		DeferredRenderPoint p;
		p.c = c;
		p.p.x = x;
		p.p.y = y;
		points_to_draw.push_back(p);
	}
}

Graphics::~Graphics() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(this->renderer);
}

void Graphics::SetColor(const Color &c) {
	SDL_SetRenderDrawColor(this->renderer, 255 * c.r, 255 * c.g, 255*c.b, 255*c.a);

}

Color Graphics::SDLColorToColor(uint32_t n) const {
	Color c = Color(0,0,0);
	c.r = (n & 0xff000000) / 255.0f;
	c.g = (n & 0xff0000) / 255.0f;
	c.b = (n & 0xff00) / 255.0f;
	return c;
}

//https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
void Graphics::Triangle(const std::array<Vec3,3>& tri, const Color& c, const Color &fill) {
/*
	std::array<Vec4,3> corrected = tri;
	(void) fill;
	Mat4 proj = Mat4::Projection();

	for(auto& v : corrected){
		auto scale = v.z;
		v = (proj * v) / scale;
	}

*/
	auto t0 = tri[0];
	auto t1 = tri[1];
	auto t2 = tri[2];

	//sort the three in order

	if (t0.y>t1.y) std::swap(t0, t1); 
	if (t0.y>t2.y) std::swap(t0, t2); 
	if (t1.y>t2.y) std::swap(t1, t2); 

	//draw lines
	LineFromVec(t0,t1,c);
	LineFromVec(t1,t2,c);
	LineFromVec(t2,t0,c);

	//get bounding box for rasterizing
	//nope jk this isn't done
	//@TODO: Do fills or whatever
}

void Graphics::Triangle(const std::array<Vec3,3>& tri, const Color& c) {
	Triangle(tri,c,c);
}



void Graphics::Polygon(const std::vector<Vertex>& poly, const Color& c) {
	std::vector<Vertex> corrected = poly;
	Mat4 proj = Mat4::Projection();

	for(auto& v : corrected){
		auto scale = v.pos.z;
		v.pos = (proj * v.pos) / scale;
	}

	for(const auto& vertex : poly){
		for(const auto vert_ref : vertex.adj){
			this->LineFromVec(vertex.pos,poly[vert_ref].pos,c);
		}
	}
}


void Graphics::Polygon(const std::vector<Vertex>& poly, const Color& c, const Quat& rotation) {
	std::vector<Vertex> corrected = poly;
	Mat4 proj = Mat4::Projection();

	for(auto& v : corrected){
		auto scale = v.pos.z;
		v.pos = (proj * v.pos) / scale;
	}

	for(const auto& vertex : poly){
		for(const auto vert_ref : vertex.adj){
			this->LineFromVec(vertex.pos.rotate(rotation),poly[vert_ref].pos.rotate(rotation),c);
		}
	}
}

void Graphics::Polygon(const std::vector<Vertex>& poly, const Color& c, const std::function<Vec4(Vec4)> transform){
	auto post_transform = poly;
	for(auto& vert : post_transform){
		vert.pos = transform(vert.pos);
	}

	for(const auto& vertex : post_transform){
		for(const auto vert_ref : vertex.adj){
			this->LineFromVec(vertex.pos,post_transform[vert_ref].pos,c);
		}
	}
}

