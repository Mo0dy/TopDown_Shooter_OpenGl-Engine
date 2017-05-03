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
	ResourceManager::LoadTexture("Textures\\HighResBackground.jpg", GL_TRUE, "background");

	renderer = new Renderer("basicShader");
	camera = new Camera;

	statEntities.push_back(new Background("background", 100));
	Players.push_back(new Player("awesomeface"));
}

void Game::ProcessInput(GLfloat dt) {
	Players.back()->movState = STOPPING;
	if (Keys[GLFW_KEY_W] == GL_TRUE) {
		Players.back()->addForce(glm::vec2(0, Players.back()->getSelfPropForce()));
		Players.back()->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_S] == GL_TRUE) {
		Players.back()->addForce(glm::vec2(0, -Players.back()->getSelfPropForce()));
		Players.back()->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_D] == GL_TRUE) {
		Players.back()->addForce(glm::vec2(Players.back()->getSelfPropForce(), 0));
		Players.back()->movState = RUNNING;
	}
	if (Keys[GLFW_KEY_A] == GL_TRUE) {
		Players.back()->addForce(glm::vec2(-Players.back()->getSelfPropForce(), 0));
		Players.back()->movState = RUNNING;
	}
}

void Game::Update(GLfloat dt) {
	LOG("FPS = " << 1 / dt);
	camera->updatePos(Width, Height, Players.back()->pos);
	for (DynE *e : dynEntities) {
		e->updateE(dt);
	}
	for (Player *e : Players) {
		e->updatePlayer(dt);
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
