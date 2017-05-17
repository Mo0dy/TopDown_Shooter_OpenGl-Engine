#pragma once

#include "CompE.h"

class SubE : public Entity
{
public:
	SubE(class CompE* masterE);
	SubE(class CompE* masterE, glm::vec2 rPos);
	SubE(class CompE* masterE, glm::vec2 rPos, const Etex* etex, GLfloat width);
	SubE(class CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec2 size);
	SubE(class CompE* masterE, glm::vec2 rPos, GLfloat rAngle);
	virtual ~SubE();

	class CompE* const masterE;
	GLboolean UpdateE(GLfloat dt);

	glm::vec2 GetRPos() const;
	GLfloat GetRAngle() const;
	
	void SetRPos(glm::vec2 rPos);
	void SetRAngle(GLfloat rAngle);

protected:
	glm::vec2 rPos;
	GLfloat rAngle;
};

