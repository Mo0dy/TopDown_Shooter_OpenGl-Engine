#include "CollisionDetector.h"
#include "Renderer.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

glm::vec2 E1corners[4];
glm::vec2 E2corners[4];
glm::vec2 Axis[4];

glm::mat2 rotMat1;
glm::mat2 rotMat2;

GLboolean CollisionDetector::doCCheck(DynE* dE, Entity* sE, GLfloat* const penDepth) {
	// Utility variables
	Hitbox mycEH;
	Hitbox mymEH;


	// This rough check only works if all hitboxes are inside the size of the texture;
	if (glm::distance(dE->pos, sE->pos) <= (glm::length(dE->size) + glm::length(sE->size)) / 2.0f) {
		for (Hitbox* mEH : dE->Hitboxes) {

			mymEH = *mEH;
			mymEH.pos = Util::create2DrotMatrix(dE->angle) * mymEH.pos + dE->pos;
			mymEH.angle += dE->angle;

			glm::mat2 rotMat1 = Util::create2DrotMatrix(mymEH.angle);
			E1corners[0] = mymEH.pos + rotMat1 * (glm::vec2(0, 1) * mymEH.size);
			E1corners[1] = mymEH.pos + rotMat1 * mymEH.size;
			E1corners[2] = mymEH.pos + rotMat1 * (glm::vec2(1, 0) * mymEH.size);
			E1corners[3] = mymEH.pos;

			Axis[0] = Util::create2DrotMatrix(mymEH.angle) * glm::vec2(1, 0);
			Axis[1] = Util::create2DrotMatrix(mymEH.angle) * glm::vec2(0, 1);

			for (Hitbox* cEH : sE->Hitboxes) {
				// Calculation WCS position of the Hitbox
				mycEH = *cEH;
				mycEH.pos = Util::create2DrotMatrix(sE->angle) * mycEH.pos + sE->pos;
				mycEH.angle += sE->angle;

				glm::mat2 rotMat2 = Util::create2DrotMatrix(mycEH.angle);

				E2corners[0] = mycEH.pos + rotMat2 * (glm::vec2(0, 1) * mycEH.size);
				E2corners[1] = mycEH.pos + rotMat2 * mycEH.size;
				E2corners[2] = mycEH.pos + rotMat2 * (glm::vec2(1, 0) * mycEH.size);
				E2corners[3] = mycEH.pos;

				Axis[2] = Util::create2DrotMatrix(mycEH.angle) * glm::vec2(1, 0);
				Axis[3] = Util::create2DrotMatrix(mycEH.angle) * glm::vec2(0, 1);

				*penDepth = doSingleCheck(mymEH, mycEH);
				if (*penDepth > 0) {
					return GL_TRUE;
				}
			}
		}
	}
	return GL_FALSE;
}

GLfloat CollisionDetector::doSingleCheck(Hitbox& h1, Hitbox& h2) {
	if (glm::distance(h1.pos, h2.pos) > (glm::length(h1.size) + glm::length(h2.size)) / 2.0f) { // rough check for possible collision
		return -1;
	}

	// Check for every axis
	// Stores min and max dist for both entities
	GLfloat E1dist[2];
	GLfloat E2dist[2];

	// This holds the signed length of the projection of the vertex vectors to the edges
	GLfloat dotProduct;
	GLfloat iDepth;
	GLfloat minIDepth = 100;

	for (glm::vec2 axis : Axis) {
		// Check all 4 corners of both entities;
		E1dist[0] = glm::dot(axis, E1corners[0]);
		E1dist[1] = E1dist[0];
		E2dist[0] = glm::dot(axis, E2corners[0]);
		E2dist[1] = E2dist[0];
		for (int i = 1; i < 4; i++) {
			dotProduct = glm::dot(axis, E1corners[i]);
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
				return -1;
			}
		}
		else {
			if (E1dist[1] < E2dist[0]) {
				return -1;
			}
		}

		// Gets the depth of the intrusion
		if (E1dist[0] > E2dist[0]) {
			iDepth = E2dist[1] - E1dist[0];
		}
		else {
			iDepth = E1dist[1] - E2dist[0];
		}
		if (iDepth < minIDepth) {
			minIDepth = iDepth;
		}
	}
	return minIDepth;
}