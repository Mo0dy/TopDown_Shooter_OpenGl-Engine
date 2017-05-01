#include "Game.h"
#include "ResourceManager.h"

Game::Game(GLuint width, GLuint height): State(GAME_ACTIVE), Width(width), Height(height)
{
}

Game::~Game()
{
	delete renderer;
	delete entity;
}

void Game::Init() {
	ResourceManager::LoadShader("myShader.vs", "myShader.frag", "basicShader");
	ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "awesomeface");

	entity = new Entity("awesomeface");
	renderer = new Renderer("basicShader");
}

void Game::ProcessInput(GLfloat dt) {
}

void Game::Update(GLfloat dt) {

}

void Game::Render() {
	renderer->DrawEntity(*entity);
}
