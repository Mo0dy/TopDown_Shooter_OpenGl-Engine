#pragma once

#include "Util.h"

#include "Texture2D.h"
#include "Animation.h"

class Entity
{
public:
	Entity();
	Entity(glm::vec2 position);
	Entity(glm::vec2 position, Etex etex);
	Entity(glm::vec2 position, GLfloat angle, Etex etex);
	Entity(glm::vec2 position, Animation ani, std::string aniName);
	Entity(glm::vec2 position, GLfloat angle, Animation ani, std::string aniName);

	virtual ~Entity();

	glm::vec2 pos; // The position in WCS coordinates
	GLfloat angle; // The angle of the entitie in radians
	glm::vec3 color;

	std::vector<Hitbox*> Hitboxes;

	Etex etex;

	std::map<std::string, Animation> Animations;

	std::string ani;

	virtual GLboolean checkForErase(glm::vec2 levelSize);
	virtual GLboolean updateE(GLfloat dt);
	virtual void setColor(glm::vec3 color);

	virtual void updateHitboxes();
	virtual void updateAni();

protected:
};

