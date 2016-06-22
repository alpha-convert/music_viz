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
	setColor(Color::White);
	SDL_RenderClear(this->renderer);
}

void Graphics::Update() {
	for(const auto &p : points_to_draw){
		setColor(p.c);
		SDL_RenderDrawPoint(renderer,p.p.x,p.p.y);
	}
	for(const auto &r : rects_to_draw){
		setColor(r.c);
		SDL_RenderDrawRect(renderer,&r.r);
	}
	for(const auto &r : filled_rects_to_draw){
		setColor(r.c);
		SDL_RenderFillRect(renderer,&r.r);
	}
	for(const auto &l : lines_to_draw){
		setColor(l.c);
		SDL_RenderDrawLine(renderer,l.x1,l.y1,l.x2,l.y2);
	}

	for(const auto &text : text_to_draw){
		SDL_Color s_c;
		s_c.r = text.c.r*255;
		s_c.g = text.c.g*255;
		s_c.b = text.c.b*255;
		s_c.a = text.c.a*255;

		auto text_surface = TTF_RenderText_Blended(text.font, text.text_str.c_str(), s_c);
		auto text_texture = SDL_CreateTextureFromSurface(renderer,text_surface); 
		assert(text_surface != nullptr);
		assert(text_texture != nullptr);

		int text_width;
		int text_height;
		auto err = TTF_SizeText(text.font, text.text_str.c_str(), &text_width, &text_height);
		assert(err == 0);

		SDL_Rect container; 
		container.x = text.x;  
		container.y = text.y; 
		container.w = text_width;
		container.h = text_height;

		SDL_RenderCopy(getRenderer(), text_texture, NULL, &container);

		SDL_FreeSurface(text_surface);
		SDL_DestroyTexture(text_texture);
	}


	SDL_UpdateWindowSurface(this->window);
	SDL_RenderPresent(this->renderer);

	points_to_draw.clear();
	rects_to_draw.clear();
	filled_rects_to_draw.clear();
	lines_to_draw.clear();
	text_to_draw.clear();
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

void Graphics::Rect(int32_t x, int32_t y, int32_t w, int32_t h, const Color &c){
	DeferredRenderRect drr;
	drr.r.x = x;
	drr.r.y = y;
	drr.r.w = w;
	drr.r.h = h;
	drr.c = c;
	rects_to_draw.push_back(drr);
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

void Graphics::Text(TTF_Font *font, const std::string &text_str, unsigned int x, unsigned int y, const Color &c){
	DeferredRenderText t;
	t.font = font;
	t.x = x;
	t.y = y;
	t.c = c;
	t.text_str= text_str;
	text_to_draw.push_back(t);
}


unsigned int Graphics::getWidth() const{
	return width;
}

unsigned int Graphics::getHeight() const{
	return height;
}

Graphics::~Graphics() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(this->renderer);
}

void Graphics::setColor(const Color &c) {
	SDL_SetRenderDrawColor(this->renderer, 255 * c.r, 255 * c.g, 255*c.b, 255*c.a);

}

Color Graphics::SDLColorToColor(uint32_t n) const {
	Color c = Color(0,0,0);
	c.r = (n & 0xff000000) / 255.0f;
	c.g = (n & 0xff0000) / 255.0f;
	c.b = (n & 0xff00) / 255.0f;
	return c;
}

SDL_Renderer *Graphics::getRenderer(){
	return renderer;
}
