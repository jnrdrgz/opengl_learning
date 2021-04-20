#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>


#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::textures;
std::map<std::string, Shader>       ResourceManager::shaders;

Shader ResourceManager::loadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name){
	shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return shaders[name];
}
Shader& ResourceManager::getShader(std::string name){
	return shaders[name];
}
Texture2D ResourceManager::loadTexture(const char* file, bool alpha, std::string name){
	textures[name] = loadTextureFromFile(file, alpha);
	return textures[name];
}
Texture2D& ResourceManager::getTexture(std::string name) {
	return textures[name];
}


void ResourceManager::clear(){
	for (auto iter : shaders) glDeleteProgram(iter.second.ID);
	for (auto iter : textures) glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile){
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	try {
		std::ifstream vertexShaderFile{ vShaderFile };
		std::ifstream fragmentShaderFile{ fShaderFile };
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}

	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}
Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha){
	Texture2D texture;
	if (alpha) {
		texture.internal_format = GL_RGBA;
		texture.image_format = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	std::cout << "w:" << width << "h:" << height << "\n";

	texture.generate(width, height, data);
	stbi_image_free(data);
	return texture;
}

