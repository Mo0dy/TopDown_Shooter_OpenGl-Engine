#pragma once

#include "Entity.h"

class StaticEntity : public Entity
{
public:
	StaticEntity();
	StaticEntity(glm::vec2 position, GLfloat width, GLfloat angle, Animation ani, std::string aniName, GLboolean concealing);
	StaticEntity(glm::vec2 position, GLfloat width, GLfloat angle, Etex etex, GLboolean concealing);
	virtual ~StaticEntity();

	GLboolean concealing;
};

