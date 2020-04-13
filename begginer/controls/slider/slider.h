#ifndef SLIDER_H
#define SLIDER_H

#include "Control.h"

class Slider : public Control
{
public:
	Slider(std::string label, float min, float max, 
			int posX, int posY, int w, int h);

	virtual bool updateControl(MouseState& state);
	virtual void drawControl(void);
	virtual std::string getType(void);

	void setValue(float* value);
protected:
	std::string label;

	float defaultValue;
	float min, max;
	float* current;


	bool dragging;




};

#endif //SLIDER