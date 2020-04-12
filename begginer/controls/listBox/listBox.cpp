#include "listBox.h"

const int itemHeight = 20;

ListBox::ListBox(int posX, int posY, int w, int h) :
	Control(posX, posY, w, h)
{
	index = 0;

}

void ListBox::addItem(std::string item){
	items.push_back(item);
}
void ListBox::removeItem(int index){
	int i = 0;

	//for (auto item : items) {
	for (std::vector<std::string>::iterator it = items.begin(); it != items.end(); it++) {
		if (i == index) {
			items.erase(it);
			break;
		}
		i++;
	}
	if (index >= (int)items.size()) {
		index = (int)items.size() - 1;
	}
}
void ListBox::setCurrent(int index){
	this->index = index;
}
int  ListBox::getIndex(void) { return index; }
int  ListBox::getCount(void) { return (int)items.size(); }


bool ListBox::updateControl(MouseState& state){
	Control::updateControl(state);

	int x = state.x;
	int y = state.y;

	if (inside == true && state.LeftButtonDown) {
		int tmpIndex = (y - posY) / itemHeight;

		if (tmpIndex >= 0 && tmpIndex < (int)items.size()) {
			index = tmpIndex;
			return true;
		}
	}

	return false;
}
void ListBox::drawControl(void){
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
		glColor4f(0.7f, 0.7f, 0.7f, 0.8f);
		glLineWidth(2.0f);
	}
	else {
		glColor4f(0.2f, 0.2f, 0.7f, 0.5f);
		glLineWidth(1.0f);
	}

	glBegin(GL_LINE_STRIP);
		glVertex2d(posX + w, posY);
		glVertex2d(posX, posY);
		glVertex2d(posX, posY + h);
		glVertex2d(posX + w, posY + h);
		glVertex2d(posX + w, posY);
	glEnd();

	if (index >= 0) {
		glColor4f(0.3f, 0.3f, 1.0f, 0.5f);

		int currentY = posY + index * itemHeight;

		glBegin(GL_QUADS);
			glVertex2d(posX + w, currentY);
			glVertex2d(posX, currentY);
			glVertex2d(posX, currentY + itemHeight);
			glVertex2d(posX + w, currentY + itemHeight);
		glEnd();

	}

	glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	
	for (int i = 0; i<items.size();i++) {
		glEngine::getEngine()->drawText(posX + 2, posY + 2 + i*itemHeight, items[i].data());
		
	}
	
}

std::string ListBox::getType(void){
	return "listBox";
}