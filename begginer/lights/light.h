#pragma once
#include <SDL_opengl.h>
#include <vector>

enum LIGHT_TYPE
{
	LIGHT_SPOT,
	LIGHT_POINT,
	LIGHT_DIRECTIONAL,
};

class Light
{
public:
	Light(LIGHT_TYPE type);
	~Light();

	static void initialize(void);

	static int numLights;
	static std::vector<int> availableLights;
	static std::vector<Light*> lights;


	void Visible(bool value = true);

	void setDiffuse(float r, float g, float b, float a);
	void setAmbient(float r, float g, float b, float a);
	void setSpecular(float r, float g, float b, float a);
	

	void setLightType(LIGHT_TYPE type);
	void setPosition(float x, float y, float z);

	//set spotlight-specific member functions
	void setSpotDirection(float x, float y, float z);
	void setCutoff(float value);
	void setExponent(float value);

	void setAttenuation(float constant, float linear, float quadratic);

	int getLight(void);
	void updateLight(void);

private:
	GLfloat position[4];
	GLfloat diffuse[4];
	GLfloat ambient[4];
	GLfloat specular[4];
	GLfloat spotDirection[4];

	float cutoff;
	float exponent;

	bool visible;
	int lightType;

	int lightNum;

};

