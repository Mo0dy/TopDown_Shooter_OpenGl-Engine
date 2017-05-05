#pragma once

#include "Util.h"

#include "Texture2D.h"
#include "myAnimation.h"

class Entity
{
public:
	Entity(glm::vec2 position);
	Entity(glm::vec2 position, glm::vec2 s);
	virtual ~Entity();
	std::string tex; // The hash of the texture stored in the Resource
	glm::vec2 size; // The size of the entities sprite in WCO coordinates (rectangle with the diagonal of size)
	glm::vec2 pos; // The position in WCS coordinates
	GLfloat angle; // The angle of the entitie in degrees
	glm::vec2 hitbox;
	myAnimation ani;

	GLboolean updateE();

	// for test
	GLboolean collision;
protected:
};

