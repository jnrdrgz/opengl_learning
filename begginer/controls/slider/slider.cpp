#include "slider.h"

const int tickSize = 5;

Slider::Slider(std::string label, float min, float max,
	int posX, int posY, int w, int h) : Control(posX, posY, w, h)
{
	this->label = label;
	defaultValue = 0.0f;
	current = nullptr;
	this->min = min;
	this->max = max;
	dragging = false;

}

void Slider::setValue(float* value) {
	current = value;
	if (current != NULL) {
		defaultValue = *current;
	}
}

bool Slider::updateControl(MouseState& state) {
	Control::updateControl(state);

	int x = state.x;
	int y = state.y;

	if (inside == true) {
		if (state.LeftButtonDown) {
			dragging = true;
		}
		if (state.RightButtonDown) {
			*current = defaultValue;
		}
	}

	if (!(state.LeftButtonDown)) {
		dragging = false;
	}

	if (dragging == true) {
		*current = (float)(x - posX)/(float)w*(max-min)+min;
		if (*current > max)
			*current = max;
		else if (*current < min) {
			*current = min;
		}
	}

	return dragging;
}
void Slider::drawControl(void) {
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

	glDisable(GL_TEXTURE_2D);

	glColor4f(0.7f, 0.7f, 0.7f, 0.8f);

	glBegin(GL_QUADS);
	glVertex2d(posX + w, posY);
	glVertex2d(posX, posY);
	glVertex2d(posX, posY + h);
	glVertex2d(posX + w, posY + h);
	glEnd();


	if (inside == true) {
		glColor4f(0.0f, 0.0f, 1.0f, 0.8f);
		glLineWidth(1.0f);
	}
	else {
		glColor4f(0.7f, 0.7f, 0.7f, 0.5f);
		glLineWidth(2.0f);
	}
	glBegin(GL_LINE_STRIP);
		glVertex2d(posX + w, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + h);
		glVertex2d(posX + w, posY + h);
		glVertex2d(posX + w, posY);
	glEnd();

	int currentX = (int)(*current - min) / (max - min) * (w - tickSize) + posX;
	
	glColor4f(0.3f, 0.3f, 1.0f, 0.5f);
	glBegin(GL_QUADS);
		glVertex2d(currentX + tickSize, posY);
		glVertex2d(currentX, posY);
		glVertex2d(currentX, posY + h);
		glVertex2d(currentX + tickSize, posY + h);
	glEnd();

	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	glEngine::getEngine()->drawText(posX + 2, posY + 2, label.data());
}
 std::string Slider::getType(void) {
	 return "slider";
}