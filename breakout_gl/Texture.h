#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Texture2D
{
	unsigned int ID{888};

	unsigned int width, height;
	unsigned int internal_format;
	unsigned int image_format;
	unsigned int wrap_s;
	unsigned int wrap_t;
	unsigned int filter_min;
	unsigned int filter_max;

	Texture2D();
	void generate(unsigned int width, unsigned int heigth, unsigned char* data);
	void bind() const;



};