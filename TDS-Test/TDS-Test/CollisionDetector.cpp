#include "CollisionDetector.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

static GLint interator = 0;

void CollisionDetector::doCCheck(std::vector<Entity*> entities) {
	for (DynE* mE : movedE) {
		for (Entity *cE : entities) {
			if (cE != mE) {
				if (doSingleCheck(mE, cE)) {
					mE->collision = GL_TRUE;
					cE->collision = GL_TRUE;

					// We should probably remove cE from mE now????? 
				}
			}
		}
	}
	movedE.clear();
}

GLboolean CollisionDetector::doSingleCheck(Entity* E1, Entity* E2) {
	if (glm::distance(E1->pos, E2->pos) > (glm::length(E1->hitbox) + glm::length(E2->hitbox)) / 2.0f) { // rough check for possible collision
		return GLU_FALSE;
	}

	// do fine collision check (Seperating Axix Theorem)
	// creating Axis to project on
	glm::vec2 Axis[4];
	Axis[0] = create2DrotMatrix(E1->angle) * glm::vec2(1, 0);
	Axis[1] = create2DrotMatrix(E1->angle) * glm::vec2(0, 1);
	Axis[2] = create2DrotMatrix(E2->angle) * glm::vec2(1, 0);
	Axis[3] = create2DrotMatrix(E2->angle) * glm::vec2(0, 1);

	// Hitbox rotated to orientation in WCS
	glm::vec2 WCS_hitBoxE1 = create2DrotMatrix(E1->angle) * E1->hitbox;
	glm::vec2 WCS_hitBoxE2 = create2DrotMatrix(E2->angle) * E2->hitbox;

	// WCS Positions of hitbox corners
	glm::vec2 E1corners[4];
	E1corners[0] = E1->pos + 0.5f * WCS_hitBoxE1;
	E1corners[1] = E1->pos - 0.5f * WCS_hitBoxE1;
	E1corners[2] = E1->pos + 0.5f * glm::vec2(-1, 1) * WCS_hitBoxE1;
	E1corners[3] = E1->pos - 0.5f * glm::vec2(-1, 1) * WCS_hitBoxE1;

	glm::vec2 E2corners[4];
	E2corners[0] = E2->pos + 0.5f * WCS_hitBoxE2;
	E2corners[1] = E2->pos - 0.5f * WCS_hitBoxE2;
	E2corners[2] = E2->pos + 0.5f * glm::vec2(-1, 1) * WCS_hitBoxE2;
	E2corners[3] = E2->pos - 0.5f * glm::vec2(-1, 1) * WCS_hitBoxE2;

	// Check for every axis
	for (glm::vec2 axis : Axis) {
		// Stores min and max dist for both entities
		GLfloat E1dist[2];
		GLfloat E2dist[2];

		// Check all 4 corners of both entities;
		E1dist[0] = glm::dot(axis, E1corners[0]);
		E1dist[1] = E1dist[0];
		E2dist[0] = glm::dot(axis, E2corners[0]);
		E2dist[1] = E2dist[0];
		for (int i = 1; i < 4; i++) {
			GLfloat dotProduct = glm::dot(axis, E1corners[i]);
			if (dotProduct > E1dist[1]) {
				E1dist[1] = dotProduct;
			}
			else if (dotProduct < E1dist[0]) {
				E1dist[0] = dotProduct;
			}
			dotProduct = glm::dot(axis, E2corners[i]);
			if (dotProduct > E2dist[1]) {
				E2dist[1] = dotProduct;
			}
			else if (dotProduct < E1dist[0]) {
				E2dist[0] = dotProduct;
			}
		}

		// Checks for Interval overlapping
		if (E1dist[1] > E2dist[1]) {
			if (E1dist[0] > E2dist[1]) {
				return GL_FALSE;
			}
		}
		else {
			if (E1dist[1] < E2dist[0]) {
				return GL_FALSE;
			}
		}
	}
	return GL_TRUE;
}

glm::mat2 CollisionDetector::create2DrotMatrix(GLfloat angle) {
	glm::mat2 rotMat;
	rotMat[0][0] = cos(angle);
	rotMat[1][0] = -sin(angle);
	rotMat[0][1] = sin(angle);
	rotMat[1][1] = cos(angle);

	return rotMat;
}

// getters and setters
void CollisionDetector::addMovedE(DynE* dE) { movedE.push_back(dE); }