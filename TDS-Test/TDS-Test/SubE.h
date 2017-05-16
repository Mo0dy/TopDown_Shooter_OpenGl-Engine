#pragma once

#include "CompE.h"

class SubE : public Entity
{
public:
	SubE(CompE* masterE);
	SubE(CompE* masterE, glm::vec2 rPos);
	SubE(CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec2 size);
	SubE(CompE* masterE, glm::vec2 rPos, GLfloat rAngle);
	virtual ~SubE();

	CompE* const masterE;

	GLboolean UpdateE(GLfloat dt);

protected:
	glm::vec2 rPos;
	GLfloat rAngle;
};

