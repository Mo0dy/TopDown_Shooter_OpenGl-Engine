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

	renderer = new Renderer("basicShader");
	camera = new Camera;

	entities.push_back(new Entity("awesomeface"));
}

void Game::ProcessInput(GLfloat dt) {

}

void Game::Update(GLfloat dt) {
	camera->updatePos();
}

void Game::Render() {
	renderer->RenderSprite(*entities.back(), *camera);
}
