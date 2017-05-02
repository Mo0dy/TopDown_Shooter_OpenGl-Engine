#include "Background.h"

Background::Background(std::string texture, float z): Entity(texture)
{
	size = glm::vec2(z, z);
}


Background::~Background()
{
}
