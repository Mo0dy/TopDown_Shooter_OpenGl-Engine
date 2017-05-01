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

const float ENTITY_SCALE = 0.01;

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
};

class Game
{
public:
	GameState State;
	GLboolean  Keys[1024];
	GLuint Width, Height;
	Game(GLuint width, GLuint height);
	~Game();

	void Init();
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	std::vector<Entity*> entities;
private:
	Camera *camera;
	Renderer *renderer;
};

