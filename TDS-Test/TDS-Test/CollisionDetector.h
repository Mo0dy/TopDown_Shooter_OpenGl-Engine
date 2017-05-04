#pragma once

#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Entity.h"
#include "DynE.h"
#include "Camera.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	void doCCheck(std::vector<Entity*> entities);

	// Getters and setters
	void addMovedE(DynE* dE);
protected:
	GLboolean doSingleCheck(Entity* E1, Entity* E2);

	std::vector<Entity*> allE; // Stores entities
	std::vector<DynE*> movedE; // Store moved entities

	glm::mat2 create2DrotMatrix(GLfloat angle);
};

