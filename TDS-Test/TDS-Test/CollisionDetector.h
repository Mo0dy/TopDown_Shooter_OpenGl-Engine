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

	GLboolean doCCheck(DynE* dE, Entity* sE, GLfloat* const penDepth, glm::vec2* const minColAxis);
protected:
	GLfloat doSingleCheck(HitObject& h1, HitObject& h2, glm::vec2* const minColAxis);
};

