#pragma once

#include "Entity.h"

class Background : public Entity
{
public:
	Background(std::string texture, glm::vec2 z); // z stands for the scale factor for the image
	~Background();
};

