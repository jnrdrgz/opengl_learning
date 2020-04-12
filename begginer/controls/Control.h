#ifndef CONTROL_H
#define CONTROL_H

#include "glEngine.h"

#include <list>
#include <string>

struct MouseState
{
	int LeftButtonDown;
	int RightButtonDown;
	int MiddleButtonDown;

	int x,y;

	MouseState() {
		LeftButtonDown = 0;
		RightButtonDown = 0;
		MiddleButtonDown = 0;

		x = 0;
		y = 0;
	}
};

class Control
{
public:
	static std::list<Control*> controls;

	Control(int posX, int posY, int w, int h);
	virtual ~Control();

	virtual bool updateControl(MouseState& state);
	virtual void drawControl(void) = 0;
	virtual std::string getType(void) = 0;
	void setPosition(int x, int y);
	void setSize(int w, int h);

	int getWidth();
	int getHeight();






protected:
	bool inside;
	int posX, posY;
	int w, h;

};

Control* addControl(Control* control);

#endif // !CONTROL_H
