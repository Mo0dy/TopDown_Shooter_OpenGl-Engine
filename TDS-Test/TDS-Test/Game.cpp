#include "Game.h"
#include "ResourceManager.h"
#include <iostream>

#define LOG(x) std::cout << x << std::endl;

Game::Game(GLuint width, GLuint height): State(GAME_ACTIVE), Width(width), Height(height)
{
}

Game::~Game()
{
	delete renderer;
	// combine this in one function (by combining all vectors)
	for (Entity *e : statEntities) {
		delete e;
	}
	for (Entity *e : dynEntities) {
		delete e;
	}
	for (Entity *e : Players) {
		delete e;
	}
	ResourceManager::Clear();
}

void Game::Init() {
	ResourceManager::LoadShader("myShader.vs", "myShader.frag", "basicShader");
	ResourceManager::LoadTexture("Textures\\awesomeface.png", GL_TRUE, "awesomeface");
	ResourceManager::LoadTexture("Textures\\Player.png", GL_TRUE, "player");
	ResourceManager::LoadTexture("Textures\\HighResBackground.jpg", GL_TRUE, "background");

	renderer = new Renderer("basicShader");
	camera = new Camera;

	statEntities.push_back(new Background("background", 100));
	Players.push_back(new Player("player"));
	Players.push_back(new Player("player"));
}

void Game::ProcessInput(GLfloat dt) {
	for (Player* p : Players) {
		p->movState = STOPPING;
	}

	// Player0
	if (Keys[GLFW_KEY_W]) {
		Players[0]->movDir += glm::vec2(0, 1);
		Players[0]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_S]) {
		Players[0]->movDir += glm::vec2(0, -1);
		Players[0]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_D]) {
		Players[0]->movDir += glm::vec2(10, 0);
		Players[0]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_A]) {
		Players[0]->movDir += glm::vec2(-1, 0);
		Players[0]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_LEFT_SHIFT]) {
		Players[0]->movState = SPRINTING;
	}

	// Player1
	if (Keys[GLFW_KEY_UP]) {
		Players[1]->movDir += glm::vec2(0, 1);
		Players[1]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_DOWN]) {
		Players[1]->movDir += glm::vec2(0, -1);
		Players[1]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_RIGHT]) {
		Players[1]->movDir += glm::vec2(1, 0);
		Players[1]->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_LEFT]) {
		Players[1]->movDir += glm::vec2(-1, 0);
		Players[1]->movState = RUNNING;
	}
}

void Game::Update(GLfloat dt) {
	//LOG("FPS = " << 1 / dt);
	camera->updatePos(Width, Height, Players);
	for (DynE *e : dynEntities) {
		e->updateE(dt);
	}
	for (Player *e : Players) {
		e->updateE(dt);
	}
}

void Game::Render() {
	// combine this in one function (by combining all vectors)
	for (Entity* e : statEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : dynEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : Players) {
		renderer->RenderSprite(*e, *camera);
	}
}
