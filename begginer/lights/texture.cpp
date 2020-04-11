#include "texture.h"

std::vector<Texture*> Texture::textures;

Texture::Texture(std::string file_name, std::string name) {
	if (!loadTGA(file_name)) {
		printf("error loading tga\n");
	}

	this->file_name = file_name; 

	textures.push_back(this);

}

Texture::~Texture(){
	for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
		if (*(it) == this) {
			textures.erase(it);
		}
	}
	if(imageData)
		delete imageData;
}

bool Texture::loadTGA(std::string file_name) {
	TGA_Header tga_header;

	std::ifstream file(file_name.data(), std::ios_base::binary);
	printf("file opened\n");

	if (!file.is_open()) return false;

	if (!file.read((char*) &tga_header, sizeof(tga_header))) return false;
	
	if (tga_header.imageType != 2) return false;

	w = tga_header.imageWidth;
	h = tga_header.imageHeight;
	bits_per_pixel = tga_header.pixelDepth;
	printf("data setted\n");

	if (w <= 0 || h <= 0 || (bits_per_pixel != 24 && bits_per_pixel != 32)) return false;
		
	GLuint type = GL_RGBA;
	if (bits_per_pixel == 24) {
		type = GL_RGB;
	}
	printf("type setted\n");

	GLuint bytesPerPixel = bits_per_pixel / 8;
	GLuint imageSize = w * h * bytesPerPixel;
	printf("size and bpp setted\n");

	imageData = new GLubyte[imageSize];
	printf("image data size alocated\n");

	if (imageData == NULL) return false;

	if(!file.read((char*)imageData, imageSize) ){
		printf("error in file read image data\n");

		//delete imageData;

		return false;
	}

	// BGR --> RGB
	/*
	for (GLuint i = 0; i < (int)imageSize; i += bytesPerPixel) {
		GLuint temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i+2] = temp;
	}*/

	createTexture(imageData, w, h, type);

	return true;

}

bool Texture::createTexture(unsigned char* imageData, int w, int h, int type) {
	glGenTextures(1, &textID);

	glBindTexture(GL_TEXTURE_2D, textID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glTexImage2D(GL_TEXTURE_2D, 0, type, w, h, 0, type, GL_UNSIGNED_BYTE, imageData);
	
	return true;
}
