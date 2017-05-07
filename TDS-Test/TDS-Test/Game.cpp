#include "Game.h"
#include "ResourceManager.h"
#include <iostream>
#include "Util.h"

#include <Xinput.h>
#include "LevelTest.h"

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Width(width), Height(height)
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
	ResourceManager::LoadShader("quadShader.vs", "quadShader.frag", "quadShader");
	ResourceManager::LoadTexture("Textures\\Util.png", GL_TRUE, "Util");
	ResourceManager::LoadTexture("Textures\\awesomeface.png", GL_TRUE, "awesomeface");

	Robot::loadRobot();
	EnergyBullet::loadEnergyBullet();
	LevelTest::loadLevelTest();

	renderer = new Renderer("basicShader");
	camera = new Camera;
	colDec = new CollisionDetector;
	level = new LevelTest;

	Players.push_back(new Robot(glm::vec2(0, -3)));
	Players.back()->setColor(glm::vec3(1.0f, 0.7f, 0.7f));

#ifdef SECOND_PLAYER
	Players.push_back(new Robot(glm::vec2(3, -3)));
	Players.back()->setColor(glm::vec3(0.7f, 0.7f, 1.0f));
#endif // SECOND_PLAYER

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			dynEntities.push_back(new DynE(glm::vec2(i * 3, j * 3)));
			dynEntities.back()->tex = "awesomeface";
			dynEntities.back()->state = MOVING;
		}
	}
}

#ifdef KEYBOARD_SUPPORT
GLboolean Press_P_Flag = false;
GLboolean Press_O_Flag = false;
#endif // KEYBOARD_SUPPORT

// UTIL
GLboolean Press_R_Flag = false;
GLboolean Press_M_Flag = false;

void Game::ProcessInput(GLfloat dt) {
	for (Player* p : Players) {
		p->state = STOPPING;
		p->wepState = NORMAL;
	}

#ifdef DEBUG
	// Util
	if (Keys[GLFW_KEY_R]) {
		Press_R_Flag = true;
	}
	if (!Keys[GLFW_KEY_R] && Press_R_Flag) {
		Press_R_Flag = false;
		reset();
	}
	if (Keys[GLFW_KEY_M]) {
		Press_M_Flag = true;
	}
	if (!Keys[GLFW_KEY_M] && Press_M_Flag) {
		Press_M_Flag = false;
		camera->minSizeHeight = CAM_STANDART_SIZE;
	}
	if (Keys[GLFW_KEY_U]) {
		camera->minSizeHeight += CAM_ZOOM_SPEED;
	}
	if (Keys[GLFW_KEY_J]) {
		camera->minSizeHeight -= CAM_ZOOM_SPEED;
	}

#endif // DEBUG

#ifdef CONTROLLER_SUPPORT
	XINPUT_STATE* cState[4];

	for (int i = 0; i < 4; i++) {
		cState[i] = new XINPUT_STATE;
	}

	// We should probably check the dwPacket number that only changes if input changes
	XINPUT_GAMEPAD gState;

	GLuint controlledPlayers = 0;

	while (getController(controlledPlayers, cState[controlledPlayers]) == ERROR_SUCCESS && controlledPlayers < 4) {
		controlledPlayers++;
	}

	if (controlledPlayers > Players.size()) {
		controlledPlayers = Players.size();
	}

	for (GLuint i = 0; i < controlledPlayers; i++) {
		gState = cState[i]->Gamepad;
		if (abs(gState.sThumbLX) > 2000 || abs(gState.sThumbLY) > 2000) {
			Players[i]->movDir += glm::vec2(gState.sThumbLX, 0);
			Players[i]->movDir += glm::vec2(0, gState.sThumbLY);
			Players[i]->state = MOVING;
		}

		if (abs(gState.sThumbRX) > 2000 || abs(gState.sThumbRY) > 2000) {
			Players[i]->bodyDir += glm::vec2(gState.sThumbRX, 0);
			Players[i]->bodyDir += glm::vec2(0, gState.sThumbRY);
			Players[i]->shoot();
		}
	}
#endif // Controller Support


#ifdef KEYBOARD_SUPPORT // KEYBORD
	// Player0
	if (Keys[GLFW_KEY_W]) {
		Players[0]->movDir += glm::vec2(0, 1);
		Players[0]->state = MOVING;
	}
	if (Keys[GLFW_KEY_S]) {
		Players[0]->movDir += glm::vec2(0, -1);
		Players[0]->state = MOVING;
	}
	if (Keys[GLFW_KEY_D]) {
		Players[0]->movDir += glm::vec2(1, 0);
		Players[0]->state = MOVING;
	}
	if (Keys[GLFW_KEY_A]) {
		Players[0]->movDir += glm::vec2(-1, 0);
		Players[0]->state = MOVING;
	}
	if (Keys[GLFW_KEY_SPACE]) {
		Players[0]->shoot();
	}

	// Player1
#ifdef SECOND_PLAYER
	if (Keys[GLFW_KEY_UP]) {
		Players[1]->movDir += glm::vec2(0, 1);
		Players[1]->state = MOVING;
	}
	if (Keys[GLFW_KEY_DOWN]) {
		Players[1]->movDir += glm::vec2(0, -1);
		Players[1]->state = MOVING;
	}
	if (Keys[GLFW_KEY_RIGHT]) {
		Players[1]->movDir += glm::vec2(1, 0);
		Players[1]->state = MOVING;
	}
	if (Keys[GLFW_KEY_LEFT]) {
		Players[1]->movDir += glm::vec2(-1, 0);
		Players[1]->state = MOVING;
	}
	if (Keys[GLFW_KEY_P]) {
		Press_P_Flag = true;
	}
	if (!Keys[GLFW_KEY_P] && Press_P_Flag) {
		Press_P_Flag = false;
		Players[1]->angle -= 0.5;
	}
	if (Keys[GLFW_KEY_O]) {
		Press_O_Flag = true;
	}
	if (!Keys[GLFW_KEY_O] && Press_O_Flag) {
		Press_O_Flag = false;
		Players[1]->angle += 0.5;
	}
	if (Keys[GLFW_KEY_RIGHT_CONTROL]) {
		Players[1]->wepState = AIMING;
}
#endif // SECOND_PLAYER
#endif // KEYBOARD_SUPPORT
}

void Game::Update(GLfloat dt) {
	//LOG("FPS = " << 1 / dt);
	camera->updatePos(Width, Height, Players);

	for (Player *e : Players) {
		for (Bullet *b : e->Bullets) {
			colDec->addMovedE(b);
		}
		if (e->updateE(dt)) {
			colDec->addMovedE(e);
		}
	}
	for (DynE *e : dynEntities) {
		if (e->updateE(dt)) {
			colDec->addMovedE(e);
		}
	}

	// This should be done at the creaton of the entites. Rewrite as soon as game structure is fixed
	std::vector<Entity*> colE;
	colE.reserve(statEntities.size() + dynEntities.size() + Players.size());
	colE.insert(colE.end(), statEntities.begin(), statEntities.end());
	colE.insert(colE.end(), dynEntities.begin(), dynEntities.end());
	for (Player* p : Players) {
		colE.push_back(p);
	}
	colDec->doCCheck(colE, dt);
}

void Game::Render() {
	// combine this in one function (by combining all vectors)
	renderer->RenderSprite(*level->background, *camera);
	for (Entity* e : statEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : dynEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Player* p : Players) {
		for (Entity* b : p->Bullets) {
			renderer->RenderSprite(*b, *camera);
		}
		for (Entity* e : p->getAddEntities()) {
			renderer->RenderSprite(*e, *camera);
		}
		renderer->RenderSprite(*p, *camera);
	}
	renderer->RenderBuffer(*camera);
}

// Utility

DWORD Game::getController(GLint index, XINPUT_STATE* state) {
	ZeroMemory(state, sizeof(XINPUT_STATE));
	return XInputGetState(index, state);
}

void Game::reset() {
	for (int i = 0; i < Players.size(); i++) {
		Players[i]->pos = glm::vec2(i * 3, -3);
	}
	for (int i = 0; i < dynEntities.size(); i++) {
		dynEntities[i]->pos = glm::vec2(i % 5 * 3, i / 5 * 3);
		dynEntities[i]->vel = glm::vec2(0);
	}
}
