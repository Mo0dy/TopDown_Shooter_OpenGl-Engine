#include "CollisionDetector.h"
#include "Renderer.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

static GLint interator = 0;

void CollisionDetector::doCCheck(std::vector<Entity*> entities, GLfloat dt) {
	// Utility variables
	Hitbox mycEH;
	Hitbox mymEH;

	while (movedE.size() > 0) {
		Entity* mE = movedE.front();
		GLboolean noCollision = GL_TRUE;
		for (Entity *cE : entities) {
			GLboolean colHappened = GL_FALSE;
			// This rough check only works if all hitboxes are inside the size of the texture;
			if (cE != mE && glm::distance(mE->pos, cE->pos) <= (glm::length(cE->size) + glm::length(mE->size)) / 2.0f) {
				Bullet* b1 = dynamic_cast<Bullet*>(mE);
				Bullet* b2 = dynamic_cast<Bullet*>(cE);
				GLboolean doColCheck = GL_TRUE;
				if (b1 != NULL && b2 != NULL) { // Two bullets
					doColCheck = GL_FALSE;
				}
				else if (b1 != NULL) {
					for (Entity* e : b1->whitelist) {
						if (cE == e) {
							doColCheck = GL_FALSE;
							break;
						}
					}
				}
				else if (b2 != NULL) {
					for (Entity* e : b2->whitelist) {
						if (mE == e) {
							doColCheck = GL_FALSE;
							break;
						}
					}
				}
				if (doColCheck) {
					for (Hitbox* mEH : mE->Hitboxes) {
						for (Hitbox* cEH : cE->Hitboxes) {
							// Calculation WCS position of the Hitbox
							mymEH = *mEH;
							mymEH.pos = Util::create2DrotMatrix(mE->angle) * mymEH.pos + mE->pos;
							mymEH.angle += mE->angle;
							mycEH = *cEH;
							mycEH.pos = Util::create2DrotMatrix(cE->angle) * mycEH.pos + cE->pos;
							mycEH.angle += cE->angle;

							if (doSingleCheck(mymEH, mycEH)) {
								noCollision = GL_FALSE;
								mE->Collision(cE, dt);
								cE->Collision(mE, dt);
								colHappened = GL_TRUE;
								break;
							}
						}
						if (colHappened) {
							break;
						}
					}
				}
			}
		}
		if (noCollision) {
			movedE.erase(movedE.begin());
		}
	}
	movedE.clear();
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

// getters and setters
void CollisionDetector::addMovedE(DynE* dE) { movedE.push_back(dE); }