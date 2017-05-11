#include "Background.h"

Background::Background(std::string texture, GLfloat width): Entity(glm::vec2(0,0), glm::vec2(width, width * ResourceManager::GetTexture(texture).Height / ResourceManager::GetTexture(texture).Width))
{
	tex = texture;
}

Background::~Background()
{
}
