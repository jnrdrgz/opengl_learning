#include "button.h"

Button::Button(std::string label, int posX, int posY, int w, int h)
	: Control(posX, posY, w, h),
	label{label}
{
	down = false;
}

bool Button::updateControl(MouseState& state) {
	Control::updateControl(state);

	if ( inside) {
		if (state.LeftButtonDown) down = true; 
		else if (down) {
			down = false;
			return true;
		}

	}

	return false;
}
void Button::drawControl(void) {
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


	if (inside) {
		glColor4f(0.7f, 0.7f, 0.7f, 0.8f);
		glLineWidth(2.0f);
	}else{
		glColor4f(0.2f, 0.2f, 0.7f, 0.5f);
		glLineWidth(2.0f);
	}
	glBegin(GL_LINE_STRIP);
		glVertex2d(posX + w, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + h);
		glVertex2d(posX + w, posY + h);
		glVertex2d(posX + w, posY);
	glEnd();

	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	int textX = posX + (w - glEngine::getEngine()->getTextWidth(label.data()))/2;
	int textY = posY + (h - glEngine::getEngine()->getTextHeight(label.data()))/2;

	glEngine::getEngine()->drawText(textX, textY, label.data());

}
std::string Button::getType(void) {
	return "button";
}