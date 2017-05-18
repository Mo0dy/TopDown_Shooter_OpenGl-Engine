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

GLboolean CollisionDetector::doCCheck(DynE* dE, Entity* sE, GLfloat* const penDepth, glm::vec2* const minColAxis) {
	// Utility variables
	HitObject mycEH;
	HitObject mymEH;


	// This rough check only works if all hitboxes are inside the size of the texture;
	//if (glm::distance(dE->GetPos(), sE->GetPos()) <= (glm::length(sE->GetSize()) + glm::length(dE->GetSize())) / 2.0f) {
		for (HitObject mEH : dE->hitObjs) {

			mymEH = mEH;
			mymEH.SetPos(Util::rotationMat2(dE->GetAngle()) * mymEH.GetPos() + dE->GetPos());
			mymEH.SetAngle(mymEH.GetAngle() + dE->GetAngle());

			// This should probably only be done if the hitboxes have the potential to collide?
			glm::mat2 rotMat = Util::rotationMat2(mymEH.GetAngle());
			glm::vec2 rotVec1 = rotMat * glm::vec2(mymEH.GetSize().x, 0) * 0.5f;
			glm::vec2 rotVec2 = rotMat * glm::vec2(0, mymEH.GetSize().y) * 0.5f;
			E1corners[0] = mymEH.GetPos() + rotVec1 + rotVec2;
			E1corners[1] = mymEH.GetPos() + rotVec1 - rotVec2;
			E1corners[2] = mymEH.GetPos() - rotVec1 - rotVec2;
			E1corners[3] = mymEH.GetPos() - rotVec1 + rotVec2;

#ifdef DEBUG_HITBOXES
			for (int i = 0; i < 3; i++) {
				Renderer::drawLineBuffer.push_back(myVertex(E1corners[i], glm::vec3(1.0f, 0.0f, 0.0f)));
				Renderer::drawLineBuffer.push_back(myVertex(E1corners[i + 1], glm::vec3(1.0f, 0.0f, 0.0f)));
			}
			Renderer::drawLineBuffer.push_back(myVertex(E1corners[3], glm::vec3(1.0f, 0.0f, 0.0f)));
			Renderer::drawLineBuffer.push_back(myVertex(E1corners[0], glm::vec3(1.0f, 0.0f, 0.0f)));
#endif // DEBUG_HITBOXES

			Axis[0] = Util::rotationMat2(mymEH.GetAngle()) * glm::vec2(1, 0);
			Axis[1] = Util::rotationMat2(mymEH.GetAngle()) * glm::vec2(0, 1);

			for (HitObject cEH : sE->hitObjs) {
				// Calculation WCS position of the Hitbox
				mycEH = cEH;
				mycEH.SetPos(Util::rotationMat2(sE->GetAngle()) * mycEH.GetPos() + sE->GetPos());
				mycEH.SetAngle(mycEH.GetAngle() + sE->GetAngle());

				rotMat = Util::rotationMat2(mycEH.GetAngle());
				rotVec1 = rotMat * glm::vec2(mycEH.GetSize().x, 0) * 0.5f;
				rotVec2 = rotMat * glm::vec2(0, mycEH.GetSize().y) * 0.5f;
				E2corners[0] = mycEH.GetPos() + rotVec1 + rotVec2;
				E2corners[1] = mycEH.GetPos() + rotVec1 - rotVec2;
				E2corners[2] = mycEH.GetPos() - rotVec1 - rotVec2;
				E2corners[3] = mycEH.GetPos() - rotVec1 + rotVec2;

#ifdef DEBUG_HITBOXES
				for (int i = 0; i < 3; i++) {
					Renderer::drawLineBuffer.push_back(myVertex(E2corners[i], glm::vec3(1.0f, 0.0f, 0.0f)));
					Renderer::drawLineBuffer.push_back(myVertex(E2corners[i + 1], glm::vec3(1.0f, 0.0f, 0.0f)));
				}
				Renderer::drawLineBuffer.push_back(myVertex(E2corners[3], glm::vec3(1.0f, 0.0f, 0.0f)));
				Renderer::drawLineBuffer.push_back(myVertex(E2corners[0], glm::vec3(1.0f, 0.0f, 0.0f)));
#endif // DEBUG_HITBOXES


				Axis[2] = Util::rotationMat2(mycEH.GetAngle()) * glm::vec2(1, 0);
				Axis[3] = Util::rotationMat2(mycEH.GetAngle()) * glm::vec2(0, 1);

				*penDepth = doSingleCheck(mymEH, mycEH, minColAxis);
				if (*penDepth > 0) {
					return GL_TRUE;
				}
			}
		}
	//}
	return GL_FALSE;
}

GLfloat CollisionDetector::doSingleCheck(HitObject& h1, HitObject& h2, glm::vec2* const minColAxis) {
	// This should probably be checked in the upper function?
	if (glm::distance(h1.GetPos(), h2.GetPos()) > (glm::length(h1.GetSize()) + glm::length(h2.GetSize())) / 2.0f) { // rough check for possible collision
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

	GLfloat axisDir; // This is either -1 or 1 making the returned axis always point from the first hitbox to the second

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
			axisDir = -1;
		}
		else {
			iDepth = E1dist[1] - E2dist[0];
			axisDir = 1;
		}
		if (iDepth < minIDepth) {
			*minColAxis = axis *  axisDir;
			minIDepth = iDepth;
		}
	}
	return minIDepth;
}