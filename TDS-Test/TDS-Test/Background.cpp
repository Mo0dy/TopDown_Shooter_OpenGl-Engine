#include "Background.h"

Background::Background(std::string texture, float z): Entity(glm::vec2(0,0), glm::vec2(z, z))
{
	tex = texture;
}


Background::~Background()
{
}
