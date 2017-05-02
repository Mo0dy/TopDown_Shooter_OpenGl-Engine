#pragma once

#include "Entity.h"

class Background : public Entity
{
public:
	Background(std::string texture, float z); // z stands for the scale factor for the image
	~Background();
};

