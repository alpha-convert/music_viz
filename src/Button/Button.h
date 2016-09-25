#pragma once
#include <functional>
#include <string>
#include "Color.h"
#include "Input.h"

/***
* @Author Joseph Cutler
* @Date June 22, 2016
* @Copyright WTFPL
*/


typedef struct button_state_t {
        bool pressed;
        std::string text;
        Color c;
} button_state_t;

class Button : public Input<button_state_t>{
public:

	static constexpr bool Down = true;
	static constexpr bool Up = false;

        Button(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char code, button_state_t startingState, const std::string &on_text, const Color &on_color);
	~Button();

	void toggleState();
        bool isPressed();

        std::string getText() const;
	Color getColor() const;
private:
	std::string on_text;
	std::string off_text;
	Color on_c;
	Color off_c;
	Color border_color;

};
