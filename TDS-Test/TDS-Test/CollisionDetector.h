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

	GLboolean DoCCheck(Entity* e1, Entity* e2, GLfloat* const penDepth, glm::vec2* const minColAxis);

	static GLboolean DoHitscan(glm::vec2 pos, glm::vec2 dir, std::vector <Entity*> entities, GLfloat *resDist);
	static GLfloat DoSingleHitscan(glm::vec2 pos, glm::vec2 dir, glm::vec2 v1, glm::vec2 v2);

protected:
	GLboolean DoPPCheck(HitPoly& hP1, HitPoly& hP2, GLfloat* colDepth, glm::vec2* const minColAxis);
	GLboolean DoCPCheck(HitCircle& hC, HitPoly& hP, GLfloat* colDepth, glm::vec2* const minColAxis);
	GLboolean DoCCCheck(HitCircle& hC1, HitCircle& hC2, GLfloat* colDepth, glm::vec2* const minColAxis);
};