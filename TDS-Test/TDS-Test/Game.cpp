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
	delete camera;
	delete colDec;
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
	ResourceManager::LoadTexture("Textures\\DrawnChar2.png", GL_TRUE, "DrawnChar2");
	ResourceManager::LoadTexture("Textures\\DrawnChar.png", GL_TRUE, "DrawnChar");
	ResourceManager::LoadTexture("Textures\\Tracks.png", GL_TRUE, "Tracks");
	ResourceManager::LoadTexture("Textures\\Terrain.png", GL_TRUE, "background");

	renderer = new Renderer("basicShader");
	camera = new Camera;
	colDec = new CollisionDetector;

	statEntities.push_back(new Background("background", 200));
	Players.push_back(new Player("DrawnChar", glm::vec2(1, 0.56758), "Tracks", 0.7f * glm::vec2(1, 0.93246)));
	Players.push_back(new Player("DrawnChar2", glm::vec2(1, 0.76836)));
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
		Players[0]->movDir += glm::vec2(1, 0);
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
		if (e->updateE(dt)) {
			colDec->addMovedE(e);
		}
	}
	for (Player *e : Players) {
		if (e->updateE(dt)) {
			colDec->addMovedE(e);
		}
	}

	// This should be done at the creaton of the entites. Rewrite as soon as game structure is fixed
	std::vector<Entity*> colE;
	colE.reserve(statEntities.size() + dynEntities.size() + Players.size());
	colE.insert(colE.end(), statEntities.begin(), statEntities.end());
	colE.insert(colE.end(), dynEntities.begin(), dynEntities.end());
	colE.insert(colE.end(), Players.begin(), Players.end());
	colDec->doCCheck(colE);
}

void Game::Render() {
	// combine this in one function (by combining all vectors)
	for (Entity* e : statEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : dynEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Player* p : Players) {
		renderer->RenderSprite(*(p->getLegs()), *camera);
		renderer->RenderSprite(*p, *camera);
	}
}
