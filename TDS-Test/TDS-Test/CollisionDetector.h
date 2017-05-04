#pragma once

#include <vector>
#include "Entity.h"
#include "DynE.h"

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	void doCCheck(std::vector<Entity*> entities);

	// Getters and setters
	void addMovedE(DynE* dE);
protected:
	std::vector<Entity*> e; // Stores entities
	std::vector<DynE*> d; // Store moved entities
};

