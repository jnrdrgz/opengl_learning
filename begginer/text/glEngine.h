#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include "texture.h"
#include "light.h"

class glEngine
{
public:
	glEngine();
	~glEngine();

	static GLvoid uninitialize(GLvoid);
	static glEngine* getEngine(GLvoid);
	
	GLvoid initialize(GLint widht, GLint height);

	GLvoid buildTextureFont(GLvoid);
	GLvoid drawText(GLint x, GLint y, const char* in_text, ...);

	GLuint getTextWidth(const char* text);
	GLuint getTextHeight(const char* text);


private:
	Texture* fontTexture;
	GLuint  fontBase;
	int fontSize = 13;
	int fontSpace = 7;


};

