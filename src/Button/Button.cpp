#include "Button.h"

Button::~Button(){}
//Assumes that off is the default state
Button::Button(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char code, button_state_t startingState, const std::string &on_text, const Color &on_color): 
Input(x,y,w,h,code,startingState){
    off_c = startingState.c;
    off_text = startingState.text;
    this->on_text = on_text;
    on_c= on_color;
}

void Button::toggleState(){
	state.pressed = !state.pressed;
}


std::string Button::getText() const{
	return state.pressed ? on_text : off_text;
}

Color Button::getColor() const{
	return state.pressed ? on_c : off_c;
}

bool Button::isPressed(){
    return state.pressed;
}
