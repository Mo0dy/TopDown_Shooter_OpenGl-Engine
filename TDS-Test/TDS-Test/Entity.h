#pragma once

#include "Util.h"

#include "Texture2D.h"
#include "Animation.h"

class Entity
{
public:
	Entity(glm::vec2 position);
	Entity(glm::vec2 position, glm::vec2 s);
	virtual ~Entity();
	std::string tex; // The hash of the texture stored in the Resource
	glm::vec2 size; // The size of the entities sprite in WCO coordinates (rectangle with the diagonal of size)
	glm::vec2 pos; // The position in WCS coordinates
	GLfloat angle; // The angle of the entitie in radians
	std::vector<Hitbox*> Hitboxes;
	Animation ani;

	virtual GLboolean updateE(GLfloat dt);
	virtual void Collision(Entity* cE, GLfloat dt);
	virtual void setColor(glm::vec3 color);
	glm::vec3 color;
protected:
};

