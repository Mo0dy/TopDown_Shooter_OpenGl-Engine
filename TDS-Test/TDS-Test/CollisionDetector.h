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
	void doSingleCheck(Entity* E1, Entity* E2);

	std::vector<Entity*> allE; // Stores entities
	std::vector<DynE*> movedE; // Store moved entities
};

