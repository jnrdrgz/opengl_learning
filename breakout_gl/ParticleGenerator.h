#pragma once

#include "glm/glm.hpp"

#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

#include <vector>

struct Particle {
	glm::vec2 position, velocity;
	glm::vec4 color;

	float life;
	Particle() :position{ 0.0f }, velocity{ 0.0f }, color{ 1.0f }, life{0.0f}{}
};


struct ParticleGenerator
{
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);

	void update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	void draw();

	std::vector<Particle> particles;
	unsigned int amount;

	Shader shader;
	Texture2D texture;

	unsigned int VAO;

	void init();
	unsigned int firstUnusedParticle();
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};