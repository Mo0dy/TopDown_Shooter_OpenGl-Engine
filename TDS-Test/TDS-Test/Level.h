#pragma once

#include "Util.h"

#include "Background.h"
#include "Entity.h"

class Level
{
public:
	Level(Background* b);
	~Level();

	Background* background;
	std::vector<Entity*> entities;
};

