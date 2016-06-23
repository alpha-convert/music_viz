/*
 * Graphics.h
 *
 *  Created on: Oct 28, 2015
 *      Author: Admin
 */

#ifndef GRAPHICS_GRAPHICS_H_
#define GRAPHICS_GRAPHICS_H_

#include <vector>
#include <string>
#include <functional>
#include <cstdint>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "Color.h"
#include "macros.h"

/**
 * @class Graphics
 * @brief Wrapper around SDL graphics
 * @author Joseph Cutler
 *
 * The SDL graphics library (while very elegant and cross platform) isn't really meant for modern C++ use.
 * It relies on a lot of pointer passing and things that you'd usually consider outdated or archaic in modern C++.
 * This class wraps the library, providing more user-friendly methods and functions.
 */

typedef struct DeferredRenderPoint{
	SDL_Point p;
	Color c;
}DeferredRenderPoint;

typedef struct DeferredRenderRect{
	SDL_Rect r;
	Color c;
} DeferredRenderRect;

typedef struct DeferredRenderLine{
	int x1, y1,x2,y2;
	Color c;
} DeferredRenderLine;

typedef struct DeferredRenderText{
	int x;
	int y;
	TTF_Font *font;
	Color c;
	std::string text_str;
} DeferredRenderText;

class Graphics {
	public:

				/**
		 * @brief Constructs and opens a window
		 *
		 * While very powerful and more user-friendly than OpenGL or D3D, SDL still requires some verbose and tedious setup before you can get to the good stuff.
		 * One of the goals of this class is to provide a one line way to safely allocate all nessicary resources and display the window. This takes advantage of C++'s wonderful RAII philosophy.
		 * This also means that the user of this class doesn't need to worry about destroy resources and memory after the window closes. All allocated memory is freed in the constructor.
		 *
		 * @param width Width of the window that is opened when this object is constructed
		 * @param height Height of the window that is opened when this object is constructed
		 * @param name Title of the window
		 */
		Graphics(uint32_t width, uint32_t height, const char *name);

		/**
		 * @brief Updates the window with the drawings created since the last update (or construction)
		 * @note This method must be called beofre any drawing will be visible.
		 */
		void Update();
		/**
		 * @brief Clear the screen
		 */
		void Clear();

		SDL_Renderer *getRenderer();

		/**
		 * @brief Draw line such that (0,0) is the center of the screen
		 * @param x1 x component of endpoint 1
		 * @param y1 y component of endpoint 1
		 * @param x2 x component of endpoint 2
		 * @param y2 y component of endpoint 2
		 */
		void SpaceLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color &c);

		/**
		 * @brief Draw a line from (x1,y1) to (x2,y2)
		 * @param x1 x coord for point 1
		 * @param y1 y coord for point 1
		 * @param x2 x coord for point 2
		 * @param y2 y coord for point 2
		 * @param c color of the line to be drawn
		 */
		void Line(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const Color &c);

		void Rect(int32_t x, int32_t y, int32_t w, int32_t h, const Color &c);

		void FillRect(int32_t x, int32_t y, int32_t w, int32_t h, const Color &c);

		/**
		 * @brief Draw a pixel at (x,y)
		 * @param x x coord of the point to draw
		 * @param y y coord of the point to draw
		 * @param c color of the point to draw
		 */
		void PutPixel(int32_t x, int32_t y, const Color &c);

		void Text(TTF_Font *font, const std::string &text_str, unsigned int x, unsigned int y, const Color &c = Color::Black);

		/**
		 * @brief Change the current draw color
		 * @param c new draw color
		 */
		inline void setColor(const Color &c);

		unsigned int getWidth() const;
		unsigned int getHeight() const;

		virtual ~Graphics();

	private:
		///Width of window
		unsigned int width;
		///Height of window
		unsigned int height;

		float vertical_angle;
		float horizontal_angle;



		std::vector<DeferredRenderPoint> points_to_draw;
		std::vector<DeferredRenderLine> lines_to_draw;
		std::vector<DeferredRenderRect> rects_to_draw;
		std::vector<DeferredRenderRect> filled_rects_to_draw;
		std::vector<DeferredRenderText> text_to_draw;
		
		SDL_Window *window;
		SDL_Renderer *renderer;
		uint32_t GetRawPixelFromSurface(uint32_t x, uint32_t y, const SDL_Surface* const surface) const;
		Color SDLColorToColor(uint32_t n) const;

};

#endif /* GRAPHICS_GRAPHICS_H_ */
