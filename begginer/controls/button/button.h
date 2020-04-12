#ifndef BUTTON_H
#define BUTTON_H

#include "Control.h"

class Button : public Control
{
public:
	Button(std::string label, int posX, int posY, int w, int h);

	virtual bool updateControl(MouseState& state);
	virtual void drawControl(void);
	virtual std::string getType(void);

protected:
	bool down;
	std::string label;
};

#endif //BUTTON_H

