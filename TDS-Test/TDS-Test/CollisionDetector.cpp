#include "CollisionDetector.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

void CollisionDetector::doCCheck(std::vector<Entity*> entities) {
	for (DynE* mE : movedE) {
		for (Entity *cE : entities) {
			
		}
	}
}

void CollisionDetector::doSingleCheck(Entity* E1, Entity* E2) {

}

// getters and setters
void CollisionDetector::addMovedE(DynE* dE) { movedE.push_back(dE); }