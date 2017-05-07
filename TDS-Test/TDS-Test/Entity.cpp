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
	if (ani.getState()) {
		tex = ani.getETex()->tex;
		size = ani.getETex()->texSize;
		if (ani.getETex()->hitboxes.size() > 0) {
			Hitboxes = ani.getETex()->hitboxes;
		}
	}
	return GL_FALSE; // static entities never move
}

void Entity::Collision(Entity* cE, GLfloat dt) {
}

void Entity::setColor(glm::vec3 color) {
	this->color = color;
}