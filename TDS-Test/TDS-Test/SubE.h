#pragma once

#include "CompE.h"

class SubE : public Entity
{
public:
	SubE(class CompE* masterE);
	SubE(class CompE* masterE, glm::vec2 rPos);
	SubE(class CompE* masterE, glm::vec2 rPos, const Etex* etex, GLfloat width, GLfloat height);
	SubE(class CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec3 size);
	SubE(class CompE* masterE, glm::vec2 rPos, GLfloat rAngle);
	virtual ~SubE();

	class CompE* const masterE;
	GLboolean UpdateE(GLfloat dt);

	glm::vec2 Get2DRPos() const;
	glm::vec3 GetRPos() const;
	GLfloat GetRAngle() const;
	
	void Set2DRPos(glm::vec2 rPos);
	void SetRPos(glm::vec3 rPos);
	void SetRAngle(GLfloat rAngle);

protected:
	virtual void UpdatePos();
	glm::vec3 rPos;
	GLfloat rAngle;
};

