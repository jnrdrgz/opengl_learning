#pragma once

#include "GameObject.h"
#include "Texture.h"

struct BallObject : GameObject
{
	float radius;
	bool stuck;

	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

	glm::vec2 move(float dt, unsigned int window_width);
	void reset(glm::vec2 position, glm::vec2 velocity);

};