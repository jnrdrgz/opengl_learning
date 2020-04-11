#include "light.h"

int Light::numLights = 0;
std::vector<int> Light::availableLights;
std::vector<Light*> Light::lights;

Light::Light(LIGHT_TYPE type) {
	lights.push_back(this);

	if ((int)availableLights.size() > 0) {
		lightNum = availableLights[0];

		availableLights.erase(availableLights.begin());

		Visible(true);

		setLightType(type);

		setPosition(0.0f, 0.0f, 0.0f);
		setSpotDirection(0.0f, -1.0f, 0.0f);

		setCutoff(45);
		setExponent(12);

		setAmbient(0, 0, 0, 1);
		setDiffuse(1, 1, 1, 1);
		setSpecular(1, 1, 1, 1);

		updateLight();
	}
	else {
		lightNum = 0;
		Visible(false);
	}
}

void Light::initialize(void) {
	glGetIntegerv(GL_MAX_LIGHTS, &numLights);
	printf("Lights: %d\n", numLights);

	for (int i = 0; i < numLights; i++) {
		availableLights.push_back(GL_LIGHT0 + i);
	}
}

Light::~Light() {
	if (lightNum != 0) {
		availableLights.push_back(lightNum);
	}

	for (std::vector<Light*>::iterator it = lights.begin(); it != lights.end(); it++) {
		if ((*it) == this) {
			lights.erase(it);
		}
	}
}

void Light::Visible(bool value){
	visible = value;

	if (visible) {
		glEnable(lightNum);
	}
	else {
		glDisable(lightNum);
	}

}

void Light::setDiffuse(float r, float g, float b, float a){
	diffuse[0] = r;
	diffuse[1] = g;
	diffuse[2] = b;
	diffuse[3] = a;

	glLightfv(lightNum, GL_DIFFUSE, diffuse);
}
void Light::setAmbient(float r, float g, float b, float a) {
	ambient[0] = r;
	ambient[1] = g;
	ambient[2] = b;
	ambient[3] = a;

	glLightfv(lightNum, GL_AMBIENT, ambient);

}
void Light::setSpecular(float r, float g, float b, float a) {
	specular[0] = r;
	specular[1] = g;
	specular[2] = b;
	specular[3] = a;

	glLightfv(lightNum, GL_SPECULAR, specular);

}
void Light::setLightType(LIGHT_TYPE type) {
	lightType = type;

	if (lightType == LIGHT_SPOT) {
		position[3] = 1.0f;
	}
	else if(lightType == LIGHT_POINT){
		position[3] = 1.0f;
		setCutoff(180.0f);
	}
	else if (lightType == LIGHT_DIRECTIONAL) {
		position[3] = 0.0f;
	}

	updateLight();

}
void Light::setPosition(float x, float y, float z) {
	position[0] = x;
	position[1] = y;
	position[2] = z;

	glLightfv(lightNum, GL_POSITION, position);
}

void Light::setSpotDirection(float x, float y, float z) {
	spotDirection[0] = x;
	spotDirection[1] = y;
	spotDirection[2] = z;

	glLightfv(lightNum, GL_SPOT_DIRECTION, position);
}

void Light::setCutoff(float value) {
	cutoff = value;

	glLightf(lightNum, GL_SPOT_CUTOFF, cutoff);
}
void Light::setExponent(float value) {
	exponent = value;

	glLightf(lightNum, GL_SPOT_EXPONENT, exponent);
}
void Light::setAttenuation(float constant, float linear, float quadratic) {
	glLightf(lightNum, GL_CONSTANT_ATTENUATION, constant);
	glLightf(lightNum, GL_LINEAR_ATTENUATION, linear);
	glLightf(lightNum, GL_QUADRATIC_ATTENUATION, quadratic);
}
int  Light::getLight(void) {
	return lightNum;
}
void Light::updateLight(void) {
	glLightfv(lightNum, GL_POSITION, position);
	glLightfv(lightNum, GL_SPOT_DIRECTION, spotDirection);
}
