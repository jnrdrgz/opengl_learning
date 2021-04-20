#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Texture.h"
#include "Shader.h"

#include <map>
#include <string>

struct ResourceManager
{
	static std::map<std::string, Shader> shaders;
	static std::map<std::string, Texture2D> textures;

	static Shader loadShader(const char* vShaderFile, const char *fShaderFile, const char* gShaderFile, std::string name);
	static Shader& getShader(std::string name);
	static Texture2D loadTexture(const char* file, bool alpha, std::string name);
	static Texture2D& getTexture(std::string name);

	static void clear();
	static Shader loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile = nullptr);
	static Texture2D loadTextureFromFile(const char* file, bool alpha);

private:
	ResourceManager(){}
};