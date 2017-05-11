#pragma once

#include "Entity.h"

class SubE : public Entity
{
public:
	SubE(glm::vec2 relativePos);
	virtual ~SubE();

	// Relative to master object
	glm::vec2 rPos; 
	GLfloat rAngle;
};

