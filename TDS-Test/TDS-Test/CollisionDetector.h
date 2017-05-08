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
#include "Util.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	GLboolean doCCheck(std::vector<Entity*> entities, GLfloat dt);

	// Getters and setters
	void addMovedE(Entity* dE);
	std::vector<Entity*> movedE; // Store moved entities
protected:
	GLboolean doSingleCheck(Hitbox& h1, Hitbox& h2);

	std::vector<Entity*> allE; // Stores entities
};

