#pragma once

#include <Windows.h>
#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <Xinput.h>

#include "GameContent\Entities\Entity.h"
#include "GameContent\Entities\DynE.h"
#include "GameContent\Entities\Enemies\Enemy.h"
#include "GameContent\Entities\Players\Player.h"
#include "GameContent\Entities\Bullets\Bullet.h"
#include "GameContent\Levels\Level.h"

#include "Maths\SightCalc.h"
#include "Maths\CollisionDetector.h"

#include "OpenGL\Renderer.h"
#include "OpenGL\Camera.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
};

class Game
{
public:
	GameState State;
	GLboolean  Keys[1024]; // Stores bool values for every key that is pressed under the corresponding index. For example if "W" is pressed Keys[GLFW_KEY_W] will be true
	GLuint Width, Height; // Size of the window at creation. !We should write an update function that allows resizing!
	Game(GLuint width, GLuint height);
	~Game();

	void Init(); // Initiates all important variables and entities ?Should we call it in the constructor?
	void ProcessInput(GLfloat dt); // Function to process inputs
	void Update(GLfloat dt); // Function to update objects
	void Render(); // Function to render objects

	// These vectors hold all entities in the game
	static std::vector<StaticEntity*> sStatEntities; // neutral static entities
	static std::vector<DynE*> sDynEntities; // neutral dynamic (moving) entities
	static std::vector<Player*> sPlayers; 
	static std::vector<Bullet*> sBullets; 
	static std::vector<Enemy*> sEnemies;
	static std::vector<DynE*> sMovedE; // Entities that moved this frame. These will actively check for collision

	static std::vector<Entity*> sSpawnE; // Entities that will be added next frame

	static void deleteEntities(); 
	static void clearEntities();

private:
	Camera *camera;
	Renderer *renderer;
	CollisionDetector *colDec;
	Level* level;
	SightCalc* sightCalc;
	

// Utility
	DWORD getController(GLint index, XINPUT_STATE* state);
	void SpawnEntities();
	void Reset();
	void CheckForErase();
	GLboolean CheckForOutOfBounds(Entity *e);
	void DoCCheck();
	void LoadEntities();
};



