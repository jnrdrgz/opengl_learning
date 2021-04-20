#include "game.h"

#include <iostream>

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "ParticleGenerator.h"

SpriteRenderer* renderer = nullptr;
GameObject* player = nullptr;

BallObject* ball = nullptr;
ParticleGenerator* particles = nullptr;

Game::Game(unsigned int width, unsigned int height)
    : state(GameState::GAME_ACTIVE), Keys(), width(width), height(height)
{

}

Game::~Game()
{
    delete renderer;
    delete player;
    delete ball;
    delete particles;
}

void Game::init()
{
    ResourceManager::loadShader("shaders/SpriteVShader.vs", "shaders/SpriteFShader.fs", nullptr, "sprite");
    ResourceManager::loadShader("shaders/particle.vs", "shaders/particle.fs", nullptr, "particle");


    std::cout << "screen" << width << "-" << height << "\n";

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->width),
        static_cast<float>(this->height), 0.0f, -1.0f, 1.0f);

    //RENDERER
    ResourceManager::getShader("sprite").use().setInteger("image", 0);
    ResourceManager::getShader("sprite").setMatrix4("projection", projection);
    
    ResourceManager::getShader("particle").use().setInteger("sprite", 0);
    ResourceManager::getShader("particle").setMatrix4("projection", projection);

    
    renderer = new SpriteRenderer(ResourceManager::getShader("sprite"));


    //TEXTURES
    ResourceManager::loadTexture("background.jpg", false, "background");
    ResourceManager::loadTexture("awesomeface.png", true, "face");
    ResourceManager::loadTexture("block.png", false, "block");
    ResourceManager::loadTexture("block_solid.png", false, "block_solid");
    ResourceManager::loadTexture("paddle.png", true, "paddle");
    ResourceManager::loadTexture("particle.png", true, "particle");

    //LEVELS
    GameLevel one;
    one.load("levels/one.lvl", this->width, this->height/2);
    GameLevel two;
    two.load("levels/two.lvl", this->width, this->height / 2);
    GameLevel three;
    three.load("levels/three.lvl", this->width, this->height / 2);
    GameLevel four;
    four.load("levels/four.lvl", this->width, this->height / 2);

    this->levels.push_back(one);
    this->levels.push_back(two);
    this->levels.push_back(three);
    this->levels.push_back(four);
    
    this->level = 0;
    
    //PLAYER INIT
    glm::vec2 playerPos = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    
    player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::getTexture("paddle"));


    //BALL

    glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS*2.0f);
    ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::getTexture("face"));


    //PARTICLES
    particles = new ParticleGenerator(
        ResourceManager::getShader("particle"),
        ResourceManager::getTexture("particle"),
        500
    );

}

void Game::update(float dt)
{
    ball->move(dt, this->width);
    this->doCollisions();

    particles->update(dt, *ball, 2, glm::vec2(ball->radius / 2.0f));

    if (ball->position.y >= this->height) {
        this->resetLevel();
        this->resetPlayer();
    }
}

void Game::resetPlayer()
{
    // reset player/ball stats
    player->size = PLAYER_SIZE;
    player->position = glm::vec2(this->width / 2.0f - PLAYER_SIZE.x / 2.0f, this->height - PLAYER_SIZE.y);
    ball->reset(player->position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

void Game::resetLevel()
{
    if (this->level == 0)
        this->levels[0].load("levels/one.lvl", this->width, this->height / 2);
    else if (this->level == 1)
        this->levels[1].load("levels/two.lvl", this->width, this->height / 2);
    else if (this->level == 2)
        this->levels[2].load("levels/three.lvl", this->width, this->height / 2);
    else if (this->level == 3)
        this->levels[3].load("levels/four.lvl", this->width, this->height / 2);
}

void Game::processInput(float dt)
{
    if (this->state == GameState::GAME_ACTIVE) {
        float velocity = PLAYER_VELOCITY * dt;

        if (this->Keys[GLFW_KEY_A])
        {
            if (player->position.x >= 0.0f) {
                player->position.x -= velocity;
                if (ball->stuck) {
                    ball->position.x -= velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (player->position.x <= this->width - player->size.x) {
                player->position.x += velocity;

                if (ball->stuck) {
                    ball->position.x += velocity;
                }
            }
        }
        if (this->Keys[GLFW_KEY_SPACE]) {
            ball->stuck = false;
        }
    }
}

void Game::render()
{
    if (this->state == GameState::GAME_ACTIVE) {
        //renderer->drawSprite(ResourceManager::getTexture("background"), glm::vec2(0.0f, 0.0f), glm::vec2(this->width, this->height), 0.0f);
        
        this->levels[this->level].draw(*renderer);
        player->draw(*renderer);
        particles->draw();
        ball->draw(*renderer);

        
    }
}

bool CheckCollision(GameObject& one, GameObject& two);
Collision CheckCollision(BallObject& one, GameObject& two);
Direction VectorDirection(glm::vec2 target);

void Game::doCollisions() {
    for (GameObject& box : this->levels[this->level].bricks) {
        if (!box.destroyed) {
            Collision collision = CheckCollision(*ball, box);

            if (std::get<0>(collision)) {

                if (!box.isSolid) {
                    box.destroyed = true;
                }

                Direction dir = std::get<1>(collision);
                glm::vec2 diff_vector = std::get<2>(collision);

                if (dir == Direction::LEFT || dir == Direction::RIGHT) {
                    ball->velocity.x = -ball->velocity.x;

                    float penetration = ball->radius - std::abs(diff_vector.x);
                    if (dir == Direction::LEFT) {
                        ball->position.x += penetration;
                    }
                    else {
                        ball->position.x -= penetration;
                    }
                }
                else {
                    ball->velocity.y = -ball->velocity.y;

                    float penetration = ball->radius - std::abs(diff_vector.y);
                    if (dir == Direction::UP) {
                        ball->position.y -= penetration;
                    }
                    else {
                        ball->position.y += penetration;
                    }

                    }
                }
            }

        }

    Collision result = CheckCollision(*ball, *player);
    if (!ball->stuck && std::get<0>(result)) {

        float centerBoard = player->position.x + player->size.x / 2.0f;
        float distance = (ball->position.x + ball->radius) - centerBoard;
        float percentage = distance / (player->size.x / 2.0f);

        float strength = 2.0f;

        glm::vec2 oldVelocity = ball->velocity;

        ball->velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        ball->velocity = glm::normalize(ball->velocity) * glm::length(oldVelocity);
        ball->velocity.y = -1.0f * abs(ball->velocity.y);
    }
}

bool CheckCollision(GameObject& one, GameObject& two) {
    // collision x-axis?
    bool collisionX = one.position.x + one.size.x >= two.position.x &&
        two.position.x + two.size.x >= one.position.x;
    // collision y-axis?
    bool collisionY = one.position.y + one.size.y >= two.position.y &&
        two.position.y + two.size.y >= one.position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

Collision CheckCollision(BallObject& one, GameObject& two) {

    using glm::vec2;

    vec2 center(one.position + one.radius);
    vec2 aabb_half_extents(two.size.x/2.0f, two.size.y/2.0f);
    vec2 aabb_center(
        two.position.x + aabb_half_extents.x, 
        two.position.y + aabb_half_extents.y
    );
    // get difference vector between both centers
    vec2 difference = center - aabb_center;
    vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // now that we know the the  clamped values, add this to AABB_center and we get the value of box closest to circle
    vec2 closest = aabb_center + clamped;
    // now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if (glm::length(difference) < one.radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
}

Direction VectorDirection(glm::vec2 target) {
    using glm::vec2;
    glm::vec2 compass[] = {
        vec2(0.0f, 1.0f),	// up
        vec2(1.0f, 0.0f),	// right
        vec2(0.0f, -1.0f),	// down
        vec2(-1.0f, 0.0f)	// left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}