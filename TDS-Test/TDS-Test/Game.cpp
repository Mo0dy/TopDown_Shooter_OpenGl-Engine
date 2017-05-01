#include "Game.h"
#include "ResourceManager.h"

Game::Game(GLuint width, GLuint height): State(GAME_ACTIVE), Width(width), Height(height)
{
}

Game::~Game()
{
	delete renderer;
	for (Entity *e : entities) {
		delete e;
	}
}

void Game::Init() {
	ResourceManager::LoadShader("myShader.vs", "myShader.frag", "basicShader");
	ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "awesomeface");

	entities.push_back(new Entity("awesomeface"));
	renderer = new Renderer("basicShader");
	camera = new Camera;
}

void Game::ProcessInput(GLfloat dt) {

}

void Game::Update(GLfloat dt) {
	camera->updatePos();
}

void Game::Render() {
	renderer->DrawEntity(*entities.back());
}
