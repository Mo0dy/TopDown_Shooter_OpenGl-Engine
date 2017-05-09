#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity(glm::vec2 position) : tex("Util"), pos(position), angle(0), size(1, 1), color(1.0f, 1.0f, 1.0f)
{
	Hitboxes.push_back(new Hitbox(-0.5f * size, size, angle));
}

Entity::Entity(glm::vec2 position, glm::vec2 s) : tex("Util"), pos(position), angle(0), size(s), color(1.0f, 1.0f, 1.0f)
{
	Hitboxes.push_back(new Hitbox(-0.5f * size, size, angle));
}

Entity::~Entity()
{
	for (Hitbox* h : Hitboxes) {
		delete h;
	}
}

GLboolean Entity::updateE(GLfloat dt) {
	return GL_FALSE; // static entities never move
}

GLboolean Entity::checkForErase(glm::vec2 levelSize) {
	if (pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}

void Entity::setColor(glm::vec3 color) {
	this->color = color;
}