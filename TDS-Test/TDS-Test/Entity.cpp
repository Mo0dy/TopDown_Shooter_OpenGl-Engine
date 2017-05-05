#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity(std::string texture, GLfloat height) : tex(texture), pos(glm::vec2(0.0f, 0.0f)), angle(0)
{
	size.y = height;
	size.x = height * ResourceManager::GetTexture(texture).Width / ResourceManager::GetTexture(texture).Height;
	hitbox = size;
	collision = GL_FALSE;
}

Entity::~Entity()
{
}
