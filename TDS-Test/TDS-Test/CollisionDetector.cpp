#include "CollisionDetector.h"
#include "Renderer.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

GLboolean CollisionDetector::doCCheck(DynE* dE, Entity* sE) {
	// Utility variables
	Hitbox mycEH;
	Hitbox mymEH;


	// This rough check only works if all hitboxes are inside the size of the texture;
	if (glm::distance(dE->pos, sE->pos) <= (glm::length(dE->size) + glm::length(sE->size)) / 2.0f) {
		for (Hitbox* mEH : dE->Hitboxes) {
			for (Hitbox* cEH : sE->Hitboxes) {
				// Calculation WCS position of the Hitbox
				mymEH = *mEH;
				mymEH.pos = Util::create2DrotMatrix(dE->angle) * mymEH.pos + dE->pos;
				mymEH.angle += dE->angle;
				mycEH = *cEH;
				mycEH.pos = Util::create2DrotMatrix(sE->angle) * mycEH.pos + sE->pos;
				mycEH.angle += sE->angle;

				if (doSingleCheck(mymEH, mycEH)) {
					return GL_TRUE;
				}
			}
		}
	}
	return GL_FALSE;
}


GLboolean CollisionDetector::doSingleCheck(Hitbox& h1, Hitbox& h2) {
	if (glm::distance(h1.pos, h2.pos) > (glm::length(h1.size) + glm::length(h2.size)) / 2.0f) { // rough check for possible collision
		return GLU_FALSE;
	}

	// do fine collision check (Seperating Axix Theorem)
	// creating Axis to project on
	glm::vec2 Axis[4];
	Axis[0] = Util::create2DrotMatrix(h1.angle) * glm::vec2(1, 0);
	Axis[1] = Util::create2DrotMatrix(h1.angle) * glm::vec2(0, 1);
	Axis[2] = Util::create2DrotMatrix(h2.angle) * glm::vec2(1, 0);
	Axis[3] = Util::create2DrotMatrix(h2.angle) * glm::vec2(0, 1);

	// Hitbox rotated to orientation in WCS
	glm::mat2 rotMat1 = Util::create2DrotMatrix(h1.angle);
	glm::mat2 rotMat2 = Util::create2DrotMatrix(h2.angle);

	// WCS Positions of hitbox corners
	glm::vec2 E1corners[4];
	E1corners[0] = h1.pos + rotMat1 * (glm::vec2(0, 1) * h1.size);
	E1corners[1] = h1.pos + rotMat1 * h1.size;
	E1corners[2] = h1.pos + rotMat1 * (glm::vec2(1, 0) * h1.size);
	E1corners[3] = h1.pos;

	glm::vec2 E2corners[4];
	E2corners[0] = h2.pos + rotMat2 * (glm::vec2(0, 1) * h2.size);
	E2corners[1] = h2.pos + rotMat2 * h2.size;
	E2corners[2] = h2.pos + rotMat2 * (glm::vec2(1, 0) * h2.size);
	E2corners[3] = h2.pos;

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
			else if (dotProduct < E2dist[0]) {
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