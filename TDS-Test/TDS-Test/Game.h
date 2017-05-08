#pragma once

#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Renderer.h"
#include "Entity.h"
#include "Camera.h"
#include "Background.h"
#include "DynE.h"
#include "Player.h"
#include "CollisionDetector.h"
#include "P_Robot.h"
#include <Windows.h>
#include <Xinput.h>
#include "Level.h"

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

	static std::vector<Entity*> statEntities; // a vector that includes all static entities
	static std::vector<DynE*> dynEntities; // a vector that includes all dynamic entities
	static std::vector<Player*> Players;
private:
	Camera *camera;
	Renderer *renderer;
	CollisionDetector *colDec;
	Level* level;

// Utility
	DWORD getController(GLint index, XINPUT_STATE* state);
	void reset();
	void checkForOutOfBounds();
	void deleteEntities();
};



