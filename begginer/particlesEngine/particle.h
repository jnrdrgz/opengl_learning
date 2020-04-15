#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector3.h"
#include <SDL_opengl.h>

class Particle
{
public:
	Particle(int id);

	void Update(long time);

public:
	int id;
	long lastTime;
	Vector3 color;

	Vector3 position;
	Vector3 velocity;
	Vector3 acceleration;
	Vector3 rotation;

	float totalLife;
	float life;

	float alpha;
	float size;

	float bounciness;

	float active;


};

#endif

