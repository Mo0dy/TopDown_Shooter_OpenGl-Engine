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
#include "..\OpenGL\Camera.h"
#include "..\GameContent\Entities\Entity.h"
#include "..\GameContent\Entities\DynE.h"
#include "..\Tools\Util.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	GLboolean DoCCheck(Entity* e1, Entity* e2, GLfloat* const penDepth, glm::vec2* const minColAxis);

	static GLboolean DoHitscan(glm::vec2 pos, glm::vec2 dir, std::vector <Entity*> entities, GLfloat *resDist);
	static GLboolean DoHitscan(glm::vec2 pos, glm::vec2 dir, std::vector <HitPoly> hitPolys, GLfloat *resDist);
	static GLboolean HitscanInBetween(glm::vec2 pos1, glm::vec2 pos2, std::vector <HitPoly> hitPolys);
	static GLboolean SingleHitscanInBetween(glm::vec2 pos1, glm::vec2 pos2, HitPoly hitPoly);
	static GLfloat DoSingleHitscan(glm::vec2 pos, glm::vec2 dir, glm::vec2 v1, glm::vec2 v2);
	static GLfloat DoSingleHitscanInBetween(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2);

protected:
	GLboolean DoPPCheck(HitPoly& hP1, HitPoly& hP2, GLfloat* colDepth, glm::vec2* const minColAxis);
	GLboolean DoCPCheck(HitCircle& hC, HitPoly& hP, GLfloat* colDepth, glm::vec2* const minColAxis);
	GLboolean DoCCCheck(HitCircle& hC1, HitCircle& hC2, GLfloat* colDepth, glm::vec2* const minColAxis);
};