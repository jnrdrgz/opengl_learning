#ifndef LISTBOX_H
#define LISTBOX_H

#include "control.h"
#include <vector>

class ListBox : public Control
{
public:
	ListBox(int posX, int posY, int w, int h);

	void addItem(std::string item);

	void removeItem(int index);
	void setCurrent(int index);

	int getIndex(void);
	int getCount(void);

	virtual bool updateControl(MouseState& state);
	virtual void drawControl(void);
	virtual std::string getType(void);

protected:
	int index;
	std::vector<std::string> items;

};

#endif //LISTBOX_H