#include "Game.h"
#include "ResourceManager.h"
#include <iostream>
#include "Util.h"

#include <Xinput.h>
#include "LevelTest.h"
#include "E_Drone.h"
#include "E_Jelly.h"
#include "LevelBanana.h"
#include "E_Medic.h"

std::vector<Entity*> Game::sStatEntities; // a vector that includes all static entities
std::vector<DynE*> Game::sDynEntities; // a vector that includes all neutral
std::vector<Enemy*> Game::sEnemies;
std::vector<Player*> Game::sPlayers;
std::vector<Bullet*> Game::sBullets;
std::vector<DynE*> Game::sMovedE;

std::vector<Entity*> Game::sSpawnE;

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
	E_Medic::Load_E_Medic();

	renderer = new Renderer("basicShader");
	camera = new Camera;
	colDec = new CollisionDetector;
	level = new LevelTest;

	Reset();
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
		Reset();
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

		if (controlledPlayers > sPlayers.size()) {
			controlledPlayers = sPlayers.size();
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
				Reset();
			}
		}

		for (GLuint i = 0; i < controlledPlayers; i++) {
			sPlayers[i]->SetGamepad(cState[i]->Gamepad);
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

	camera->updatePos(Width, Height, sPlayers);

	for (Player *e : sPlayers) {
		e->UpdateE(dt);
	}
	for (Bullet *e : sBullets) {
		e->UpdateE(dt);
	}
	for (Enemy *e : sEnemies) {
		e->UpdateE(dt);
	}
	for (DynE *e : sDynEntities) {
		if (e->UpdateE(dt)) {
			sMovedE.push_back(e);
		}
	}
	level->UpdateL(dt);

	// Collision detection
	GLfloat penDepth;
	glm::vec2 colAxis;
	for (int i = 0; i < sPlayers.size(); i++) {
		for (Enemy *e : sEnemies) {
			if (colDec->doCCheck(sPlayers[i], e, &penDepth, &colAxis)) {
				sPlayers[i]->ColWithDyn(e, penDepth, colAxis);
				e->ColWithPlayer(sPlayers[i], penDepth, -colAxis);
			}
		}
		for (int j = i + 1; j < sPlayers.size(); j++) {
			if (colDec->doCCheck(sPlayers[i], sPlayers[j], &penDepth, &colAxis)) {
				sPlayers[i]->ColWithDyn(sPlayers[j], penDepth, colAxis);
				sPlayers[j]->ColWithDyn(sPlayers[i], penDepth, -colAxis);
			}
		}
		for (Entity *e : level->entities) {
			if (colDec->doCCheck(sPlayers[i], e, &penDepth, &colAxis)) {
				sPlayers[i]->ColWithStat(e, penDepth, colAxis);
			}
		}
		if (colDec->doCCheck(sPlayers[i], &level->background, &penDepth, &colAxis)) {
			sPlayers[i]->ColWithStat(&level->background, penDepth, colAxis);
		}
	}

	// We should probably only check for all Enemies that moved but for now this is fine
	for (int i = 0; i < sEnemies.size(); i++) {
		for (int j = i + 1; j < sEnemies.size(); j++) {
			if (colDec->doCCheck(sEnemies[i], sEnemies[j], &penDepth, &colAxis)) {
				sEnemies[i]->ColWithDyn(sEnemies[j], penDepth, colAxis);
				sEnemies[j]->ColWithDyn(sEnemies[i], penDepth, -colAxis);
			}
		}
		for (Entity *e : level->entities) {
			if (colDec->doCCheck(sEnemies[i], e, &penDepth, &colAxis)) {
				sEnemies[i]->ColWithStat(e, penDepth, colAxis);
			}
		}
	}

	for (Bullet *b : sBullets) {
		for (Enemy *e : sEnemies) {
			if (colDec->doCCheck(b, e, &penDepth, &colAxis)) {
				b->ColWithEnemy(e, penDepth, colAxis);
				e->ColWithDyn(b, 0, -colAxis);
			}
		}
		for (Entity *e : level->entities) {
			if (colDec->doCCheck(b, e, &penDepth, &colAxis)) {
				b->ColWithStat(e, penDepth, colAxis);
			}
		}
	}

	sMovedE.clear();
	CheckForErase();
}

void Game::Render() {
	// combine this in one function (by combining all vectors)
	renderer->RenderSprite(level->background, *camera);
	for (Entity* e : sStatEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : sBullets) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : sDynEntities) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Entity* e : sEnemies) {
		renderer->RenderSprite(*e, *camera);
	}
	for (Player* p : sPlayers) {
		for (std::string s : p->renderOrder) {
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

void Game::Reset() {
	level->Reset();
}

void Game::CheckForErase() {
	for (int i = 0; i < sBullets.size(); i++) {
		if (sBullets[i]->GetErase()) {
			delete sBullets[i];
			sBullets.erase(sBullets.begin() + i);
		}
	}
	for (int i = 0; i < sPlayers.size(); i++) {
		if (sPlayers[i]->GetErase()) {
			delete sPlayers[i];
			sPlayers.erase(sPlayers.begin() + i);
		}
	}
	for (int i = 0; i < sDynEntities.size(); i++) {
		if (sDynEntities[i]->GetErase()) {
			delete sDynEntities[i];
			sDynEntities.erase(sDynEntities.begin() + i);
		}
	}
	for (int i = 0; i < sEnemies.size(); i++) {
		if (sEnemies[i]->GetErase()) {
			delete sEnemies[i];
			sEnemies.erase(sEnemies.begin() + i);
		}
	}
}

void Game::deleteEntities() {
	for (Entity *e : sStatEntities) {
		delete e;
	}
	for (Entity *e : sDynEntities) {
		delete e;
	}
	for (Entity *e : sPlayers) {
		delete e;
	}
	for (Entity *e : sBullets) {
		delete e;
	}
	for (Entity *e : sEnemies) {
		delete e;
	}
}

void Game::clearEntities() {
	sPlayers.clear();
	sStatEntities.clear();
	sDynEntities.clear();
	sBullets.clear();
	sEnemies.clear();
	sMovedE.clear();
	sSpawnE.clear();
}