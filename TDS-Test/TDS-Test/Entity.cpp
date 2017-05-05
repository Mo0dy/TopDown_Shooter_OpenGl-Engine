#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity(glm::vec2 position) : tex("Util"), pos(position), angle(0), size(1, 1), hitbox(size)
{
	collision = GL_FALSE;
}

Entity::Entity(glm::vec2 position, glm::vec2 s) : tex("Util"), pos(position), angle(0), size(s), hitbox(size) {
	collision = GL_FALSE;
}

Entity::~Entity()
{
}

GLboolean Entity::updateE() {
	if (ani.getState) {
		tex = ani.getETex()->tex;
	}
	return GL_FALSE; // static entities never move
}
