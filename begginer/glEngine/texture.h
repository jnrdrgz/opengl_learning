#pragma once

#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

struct TGA_Header
{
	GLubyte ID_Length;
	GLubyte colorMapType;
	GLubyte imageType;
	GLubyte colorMapSpecification[5];
	GLshort xOrigin;
	GLshort yOrigin;
	GLshort imageWidth;
	GLshort imageHeight;
	GLubyte pixelDepth;
};

class Texture
{
public:
	Texture(std::string file_name, std::string name);
	~Texture();

	unsigned char* imageData = nullptr;
	unsigned int bits_per_pixel;
	unsigned int h, w, textID;
	std::string file_name;

	static std::vector <Texture*> textures;

private:
	bool loadTGA(std::string file_name);
	bool createTexture(unsigned char* imageData, int w, int h, int type);

};

