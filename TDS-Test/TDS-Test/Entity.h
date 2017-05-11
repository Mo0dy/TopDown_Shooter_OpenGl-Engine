#pragma once

#include "Util.h"

#include "Texture2D.h"
#include "Animation.h"

class Entity
{
public:
	Entity();
	Entity(glm::vec2 position);
	Entity(glm::vec2 position, glm::vec2 size);
	Entity(glm::vec2 position, glm::vec2 size, GLfloat angle);
	Entity(glm::vec2 position, glm::vec2 size, std::string texture);
	Entity(glm::vec2 position, GLfloat width, std::string texture);
	Entity(glm::vec2 position, GLfloat width, GLfloat angle, std::string texture);
	Entity(glm::vec2 position, glm::vec2 size, GLfloat angle, std::string texture);

	virtual ~Entity();
	std::string tex; // The hash of the texture stored in the Resource
	glm::vec2 size; // The size of the entities sprite in WCO coordinates (rectangle with the diagonal of size)
	glm::vec2 pos; // The position in WCS coordinates
	GLfloat angle; // The angle of the entitie in radians
	std::vector<Hitbox*> Hitboxes;
	glm::vec3 color;
	std::map<std::string, Animation*> Animations;

	virtual GLboolean checkForErase(glm::vec2 levelSize);
	virtual GLboolean updateE(GLfloat dt);
	virtual void setColor(glm::vec3 color);

	void Entity::autofitHitbox();
protected:
};

