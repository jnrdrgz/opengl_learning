#pragma once

#include <vector>

#include "GameObject.h"
#include "ResourceManager.h"

#include "glm/glm.hpp"


struct GameLevel
{

	std::vector<GameObject> bricks;
	GameLevel(){}

	void load(const char* file, unsigned int levelWidth, unsigned int levelHeight);
	void draw(SpriteRenderer& renderer);

	bool isCompleted();

	private:
		void init(std::vector<std::vector<unsigned int>> tileData,
			unsigned int lvlWidth, unsigned int lvlHeight);
};