#include "Entity.h"
#include "ResourceManager.h"


Entity::Entity(std::string texture): myTexture(texture),size(1,1), pos(glm::vec2(0.5f,0.5f)), angle(0)
{
	
}


Entity::~Entity()
{
}
