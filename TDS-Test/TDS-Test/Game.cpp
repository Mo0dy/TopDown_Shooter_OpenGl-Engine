#include "Game.h"
#include "ResourceManager.h"
#include <iostream>
#include "Util.h"

#include <Xinput.h>
#include "LevelTest.h"
#include "E_Drone.h"
#include "E_Jelly.h"
#include "LevelBanana.h"

std::vector<Entity*> Game::statEntities; // a vector that includes all static entities
std::vector<DynE*> Game::dynEntities; // a vector that includes all neutral
std::vector<Enemy*> Game::Enemies;
std::vector<Player*> Game::Players;
std::vector<Bullet*> Game::Bullets;
std::vector<DynE*> Game::movedE;

Game::Game(GLuint width, GLuint height) : State(GAME_ACTIVE), Width(width), Height(height)
{
}

Game::~Game()
{
	delete renderer;
	delete camera;
	delete colDec;
	// combine this in one function (by combining all vectors)
	deleteEntities();
	ResourceManager::Clear();
}

void Game::Init() {
	ResourceManager::LoadShader("myShader.vs", "myShader.frag", "basicShader");
	ResourceManager::LoadShader("quadShader.vs", "quadShader.frag", "quadShader");
	ResourceManager::LoadTexture("Textures\\Util.png", GL_TRUE, "Util");

	E_Drone::load_E_Drone();
	Robot::loadRobot();
	EnergyBullet::loadEnergyBullet();
	LevelTest::loadLevelTest();
	LevelBanana::loadLevelBanana();
	E_Jelly::Load_E_Jelly();

	renderer = new Renderer("basicShader");
	camera = new Camera;
	colDec = new CollisionDetector;
	level = new LevelBanana;

	reset();
}

#ifdef KEYBOARD_SUPPORT
GLboolean Press_P_Flag = GL_FALSE;
GLboolean Press_O_Flag = GL_FALSE;
#endif // KEYBOARD_SUPPORT

// UTIL
GLboolean Press_R_Flag = GL_FALSE;
GLboolean Press_M_Flag = GL_FALSE;

void Game::ProcessInput(GLfloat dt) {

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
		camera->minSizeHeight = Util::CAM_STANDARD_MIN_ZOOM;
	}
	if (Keys[GLFW_KEY_U]) {
		camera->minSizeHeight -= Util::CAM_ZOOM_SPEED;
	}
	if (Keys[GLFW_KEY_J]) {
		camera->minSizeHeight += Util::CAM_ZOOM_SPEED;
	}

#endif // DEBUG

	if (Util::CONTROLLER_SUPPORT) {
		XINPUT_STATE* cState[4];

		for (int i = 0; i < 4; i++) {
			cState[i] = new XINPUT_STATE;
		}

		// We should probably check the dwPacket number that only changes if input changes
		GLuint controlledPlayers = 0;

		while (getController(controlledPlayers, cState[controlledPlayers]) == ERROR_SUCCESS && controlledPlayers < 4) {
			controlledPlayers++;
		}

		if (controlledPlayers > Players.size()) {
			controlledPlayers = Players.size();
		}

		for (int i = 0; i < controlledPlayers; i++) {
			if (cState[i]->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				camera->minSizeHeight -= Util::CAM_ZOOM_SPEED;
				if (camera->minSizeHeight < 6) {
					camera->minSizeHeight = 6;
				}
			}
			if (cState[i]->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				camera->minSizeHeight += Util::CAM_ZOOM_SPEED;
				if (camera->minSizeHeight > Util::CAM_MAX_ZOOM) {
					camera->minSizeHeight = Util::CAM_MAX_ZOOM;
				}
			}
			if (cState[i]->Gamepad.wButtons & XINPUT_GAMEPAD_START) {
				reset();
			}
		}

		for (GLuint i = 0; i < controlledPlayers; i++) {
			Players[i]->gPad = cState[i]->Gamepad;
		}
	}


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

#ifdef LOG_FPS
	LOG("FPS = " << 1 / dt);
#endif //LOG_FPS

	camera->updatePos(Width, Height, Players);

	for (Player *e : Players) {
		e->updateE(dt);
	}
	for (Bullet *e : Bullets) {
		e->updateE(dt);
	}
	for (Enemy *e : Enemies) {
		e->updateE(dt);
	}
	for (DynE *e : dynEntities) {
		if (e->updateE(dt)) {
			movedE.push_back(e);
		}
	}
	level->updateL(dt);

	// Collision detection
	GLfloat penDepth;
	glm::vec2 colAxis;
	for (int i = 0; i < Players.size(); i++) {
		for (Enemy *e : Enemies) {
			if (colDec->doCCheck(Players[i], e, &penDepth, &colAxis)) {
				Players[i]->ColWithDyn(e, penDepth, colAxis);
				e->ColWithPlayer(Players[i], penDepth, -colAxis);
			}
		}
		for (int j = i + 1; j < Players.size(); j++) {
			if (colDec->doCCheck(Players[i], Players[j], &penDepth, &colAxis)) {
				Players[i]->ColWithDyn(Players[j], penDepth, colAxis);
				Players[j]->ColWithDyn(Players[i], penDepth, -colAxis);
			}
		}
		for (Entity *e : level->entities) {
			if (colDec->doCCheck(Players[i], e, &penDepth, &colAxis)) {
				Players[i]->ColWithStat(e, penDepth, colAxis);
			}
		}
		if (colDec->doCCheck(Players[i], &level->background, &penDepth, &colAxis)) {
			Players[i]->ColWithStat(&level->background, penDepth, colAxis);
		}
	}

	// We should probably only check for all Enemies that moved but for now this is fine
	for (int i = 0; i < Enemies.size(); i++) {
		for (int j = i + 1; j < Enemies.size(); j++) {
			if (colDec->doCCheck(Enemies[i], Enemies[j], &penDepth, &colAxis)) {
				Enemies[i]->ColWithDyn(Enemies[j], penDepth, colAxis);
				Enemies[j]->ColWithDyn(Enemies[i], penDepth, -colAxis);
			}
		}
		for (Entity *e : level->entities) {
			if (colDec->doCCheck(Enemies[i], e, &penDepth, &colAxis)) {
				Enemies[i]->ColWithStat(e, penDepth, colAxis);
			}
		}
	}

	for (Bullet *b : Bullets) {
		for (Enemy *e : Enemies) {
			if (colDec->doCCheck(b, e, &penDepth, &colAxis)) {
				b->ColWithEnemy(e);
				e->ColWithDyn(b, 0, -colAxis);
			}
		}
		for (Entity *e : level->entities) {
			if (colDec->doCCheck(b, e, &penDepth, &colAxis)) {
				b->ColWithStat(e, 0);
			}
		}
	}

	movedE.clear();

	checkForErase();
}

void Game::Render() {
	// combine this in one function (by combining all vectors)
	renderer->RenderSprite(level->background, *camera);
	for (Entity* e : statEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : Bullets) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : dynEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : Enemies) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Player* p : Players) {
		for (std::string s : p->renderList) {
			renderer->RenderSprite(*p->subEntities[s], *camera);
		}
	}
	for (Entity* e : level->entities) {
		renderer->RenderSprite(*e, *camera);
	}
	renderer->RenderBuffer(*camera);
}

// Utility
DWORD Game::getController(GLint index, XINPUT_STATE* state) {
	ZeroMemory(state, sizeof(XINPUT_STATE));
	return XInputGetState(index, state);
}

void Game::reset() {
	level->reset();
}

void Game::checkForErase() {
	for (int i = 0; i < Bullets.size(); i++) {
		if (Bullets[i]->checkForErase(level->size)) {
			delete Bullets[i];
			Bullets.erase(Bullets.begin() + i);
		}
	}
	for (int i = 0; i < Players.size(); i++) {
		if (Players[i]->checkForErase(level->size)) {
			Players[i]->pos = glm::vec2(0);
			Players[i]->death = GL_TRUE;
		}
		if (Players[i]->death && Players.size() > 1) {
			delete Players[i];
			Players.erase(Players.begin() + i);
		}
	}
	for (int i = 0; i < dynEntities.size(); i++) {
		if (dynEntities[i]->checkForErase(level->size)) {
			delete dynEntities[i];
			dynEntities.erase(dynEntities.begin() + i);
		}
	}
	for (int i = 0; i < Enemies.size(); i++) {
		if (Enemies[i]->checkForErase(level->size)) {
			delete Enemies[i];
			Enemies.erase(Enemies.begin() + i);
		}
	}
}

void Game::deleteEntities() {
	for (Entity *e : statEntities) {
		delete e;
	}
	for (Entity *e : dynEntities) {
		delete e;
	}
	for (Entity *e : Players) {
		delete e;
	}
	for (Entity *e : Bullets) {
		delete e;
	}
	for (Entity *e : Enemies) {
		delete e;
	}
}

void Game::clearEntities() {
	Players.clear();
	statEntities.clear();
	dynEntities.clear();
	Bullets.clear();
	Enemies.clear();
}