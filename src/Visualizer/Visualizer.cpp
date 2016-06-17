#include "Visualizer.h"
Visualizer::Visualizer(Graphics *g, Audio *a){
	this->g = g;
	this->a = a;

	request_draw = false;
	request_update = false;
}

