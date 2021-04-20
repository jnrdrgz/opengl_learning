
#include "BallObject.h"

BallObject::BallObject() : GameObject(), radius{12.5f}, stuck{true}{

}
BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) :
	GameObject(pos, glm::vec2(radius*2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity),
	radius{ radius }, stuck{true}
{

}

glm::vec2 BallObject::move(float dt, unsigned int window_width) {
	glm::vec2 pos = this->position;
	glm::vec2 vel = this->velocity;
	glm::vec2 size = this->size;
	if (!this->stuck) {
		pos += vel * dt;
		if (pos.x <= 0.0f) {
			vel.x = -vel.x;
			pos.x = 0.0f;
		}
		else if (pos.x + size.x >= window_width){
			vel.x = -vel.x;
			pos.x = window_width - size.x;
		}

		if (pos.y <= 0.0f) {
			vel.y = -vel.y;
			pos.y = 0.0f;
		}

		this->position = pos;
		this->velocity = vel;
		this->size = size;
		return this->position;
	}
}
void BallObject::reset(glm::vec2 position, glm::vec2 velocity) {
	this->position = position;
	this->velocity = velocity;
	this->stuck = true;
}