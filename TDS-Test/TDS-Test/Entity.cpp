#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity(std::string texture, glm::vec2 s) : tex(texture), size(s), pos(glm::vec2(0.0f, 0.0f)), angle(0), hitbox(size)
{
	collision = GL_FALSE;
}

Entity::~Entity()
{
}
