#include "Background.h"

Background::Background(std::string texture, glm::vec2 z): Entity(glm::vec2(0,0), z)
{
	tex = texture;
}


Background::~Background()
{
}
