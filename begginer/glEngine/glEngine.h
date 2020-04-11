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
	static GLvoid initialize(GLint widht, GLint height);
	static glEngine* getEngine(GLvoid);
private:
};

