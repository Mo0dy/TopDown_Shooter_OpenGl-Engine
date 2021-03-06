#pragma once

#include "Util.h"

class HitObject
{
public:
	HitObject();
	HitObject(glm::vec2 pos, glm::vec2 size, GLfloat angle);
	~HitObject();

	// Getters and setters
	glm::vec2 GetPos();
	glm::vec2 GetSize();
	GLfloat GetAngle();

	void SetPos(glm::vec2 pos);
	void SetSize(glm::vec2 size);
	void SetAngle(GLfloat angle);

protected:
	glm::vec2 pos; // center of the hitbox proportionate to the size of the texture
	glm::vec2 size; // Size of the rectangle from lower left corner as proportion to the width and height;
	GLfloat angle; // angle relative to the corresponding texture
};

