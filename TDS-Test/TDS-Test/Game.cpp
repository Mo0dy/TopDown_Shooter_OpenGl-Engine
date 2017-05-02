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
	for (Entity *e : statEntities) {
		delete e;
	}
	for (Entity *e : dynEntities) {
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
	dynEntities.push_back(new DynE("awesomeface"));
}

void Game::ProcessInput(GLfloat dt) {
	dynEntities.back()->stopFlag = true;
	if (Keys[GLFW_KEY_W] == GL_TRUE) {
		dynEntities.back()->addForce(glm::vec2(0, dynEntities.back()->getSelfPropForce()));
		dynEntities.back()->stopFlag = false;
	}
	if (Keys[GLFW_KEY_S] == GL_TRUE) {
		dynEntities.back()->addForce(glm::vec2(0, -dynEntities.back()->getSelfPropForce()));
		dynEntities.back()->stopFlag = false;
	}
	if (Keys[GLFW_KEY_D] == GL_TRUE) {
		dynEntities.back()->addForce(glm::vec2(dynEntities.back()->getSelfPropForce(), 0));
		dynEntities.back()->stopFlag = false;
	}
	if (Keys[GLFW_KEY_A] == GL_TRUE) {
		dynEntities.back()->addForce(glm::vec2(-dynEntities.back()->getSelfPropForce(), 0));
		dynEntities.back()->stopFlag = false;
	}
}

void Game::Update(GLfloat dt) {
	LOG("FPS = " << 1 / dt);
	camera->updatePos(Width, Height, dynEntities.back()->pos);
	for (DynE *e : dynEntities) {
		e->doStep(dt);
	}
}

void Game::Render() {
	for (Entity* e : statEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : dynEntities) {
		renderer->RenderSprite(*e, *camera);
	}
}
