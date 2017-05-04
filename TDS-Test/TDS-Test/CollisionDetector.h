#pragma once

#include <vector>
#include "Entity.h"

class CollisionDetector
{
public:
	CollisionDetector(std::vector<Entity*> entities);
	~CollisionDetector();

protected:
	std::vector<Entity*> e; // Stores entities

};

