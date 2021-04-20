#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "GameLevel.h"
#include "BallObject.h"

#include "glm/glm.hpp"

enum class GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
};

enum class Direction {
	UP,RIGHT,DOWN,LEFT
};

// <collision?, what direction?, difference vector center - closest point>
typedef std::tuple<bool, Direction, glm::vec2> Collision;

struct s_Collision {
	s_Collision() : collision{ false }, direction{ Direction::UP }, diffVector{ 0.0f,0.0f }{}
	s_Collision(bool c, Direction d, glm::vec2 dv) : collision{ c }, direction{ d }, diffVector{ dv }{}
	bool collision;
	Direction direction;
	glm::vec2 diffVector;
};

const glm::vec2 PLAYER_SIZE{ 100.0f, 20.0f };
const float PLAYER_VELOCITY{ 500.0f };

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;


struct Game
{
	GameState state;
	bool Keys[1024];
	unsigned int width, height;

	std::vector<GameLevel> levels;
	unsigned int level;

	Game(unsigned int width, unsigned int height);
	~Game();

	void init();
	void processInput(float dt);
	void update(float dt);
	void render();

	void doCollisions();

	void resetLevel();
	void resetPlayer();
};