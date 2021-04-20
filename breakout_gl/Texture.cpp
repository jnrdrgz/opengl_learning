#include "Texture.h"

#include "Debug.h"


Texture2D::Texture2D()
	: width(0), height(0), internal_format(GL_RGB), image_format(GL_RGB), wrap_s(GL_REPEAT), 
	wrap_t(GL_REPEAT), filter_min(GL_LINEAR), filter_max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}
void Texture2D::generate(unsigned int width, unsigned int height, unsigned char* data) {
	this->width = width;
	this->height = height;

	//create texture
	GLCall(glBindTexture(GL_TEXTURE_2D, this->ID));
	
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format, width, height, 0, this->image_format, GL_UNSIGNED_BYTE, data));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap_s));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap_t));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter_min));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter_max));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
void Texture2D::bind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, this->ID));
}
