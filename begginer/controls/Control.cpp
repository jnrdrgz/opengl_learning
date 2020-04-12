#include "control.h"

std::list<Control*> Control::controls;

Control::Control(int posX, int posY, int w, int h)
	: posX{posX},
	posY{posY},
	w{w},
	h{h}{

	controls.push_back(this);
}

Control::~Control() {
	controls.remove(this);
}

bool Control::updateControl(MouseState& state) {
	int x = state.x;
	int y = state.y;

	inside = false;
	if (x >= posX && x <= posX + w &&
		y >= posY && x <= posY + h) {
		inside = true;
	}

	return false;


}

void Control::setPosition(int x, int y){
	posX = x;
	posY = y;
}
void Control::setSize(int w, int h) {
	this->w = w;
	this->h = h;

}

int Control::getWidth(){
	return w;
}
int Control::getHeight(){ 
	return h;
}

Control* addControl(Control* control) {
	static int lastX = 5;
	static int lastY = 5;

	control->setPosition(lastX, lastY);
	lastY += control->getHeight() + 5;

	return control;
}