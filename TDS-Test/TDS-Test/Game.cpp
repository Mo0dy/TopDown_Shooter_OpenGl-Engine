#include "Game.h"
#include "Renderer.h"
#include "ResourceManager.h"

Renderer *renderer;
Entity *entity;

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
	ResourceManager::GetShader("basicShader").Use();

	entity = new Entity();
	renderer = new Renderer(ResourceManager::GetShader("basicShader"));
}

void Game::ProcessInput(GLfloat dt) {
}

void Game::Update(GLfloat dt) {

}

void Game::Render() {
	renderer->DrawEntity(*entity);
}
