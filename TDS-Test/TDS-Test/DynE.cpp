#include "DynE.h"

#include <iostream>
#include "Renderer.h"

DynE::DynE(glm::vec2 position) : Entity(position), mass(1), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-0.1f)
{
}

DynE::~DynE()
{
}

GLboolean DynE::updateE(float dt) {
	// updating animation
	if (ani.getState()) {
		tex = ani.getETex()->tex;
		size = ani.getETex()->texSize;
		if (ani.getETex()->hitboxes.size() > 0) {
			Hitboxes = ani.getETex()->hitboxes;
		}
	}

	glm::vec2 dV = dt * (force + fricRes() + airRes()) / mass;

	// safeguard for wiggeling close to 0v
	if (vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
		vel = glm::vec2(0, 0);
	} else {
		vel += dV;
	}

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
	return glm::length(vel) > 0;
}

void DynE::Collision(Entity* cE, GLfloat dt) {
	DynE* E2 = dynamic_cast<DynE*> (cE);
	if (E2 == NULL) { // Collision with static object
		
	}
	else { // Collision with dynamic object
		glm::vec2 collisionForce = (glm::dot((E2->pos - pos), E2->vel) * E2->mass / glm::pow(glm::distance(pos, E2->pos), 2) / dt) * (E2->pos - pos);
		addForce(collisionForce);
		Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
		Renderer::drawLineBuffer.push_back(myVertex(pos + collisionForce * 0.001f, glm::vec3(1.0f, 1.0f, 0.0f)));
	}
}

// Utitlity functions
glm::vec2 DynE::fricRes() {
	if (glm::length(vel) > 0) {
		return glm::normalize(vel) * dynFricCoeff * mass * GRAV_ACC; // Dynamic Friction
	}
	else {
		if (glm::length(force) > 0) {
			return glm::normalize(force) * statFricCoeff * mass * GRAV_ACC; // Static Friction
		}
		else {
			return glm::vec2(0, 0);
		}
	}
}

glm::vec2 DynE::airRes() {
	if (glm::length(vel) > 0) {
		return glm::normalize(vel) * glm::pow(glm::length(vel), 2) * airFricCoeff;
	}
	return glm::vec2(0,0);
}

// Getters and Setters

void DynE::addForce(glm::vec2 f) {
	force += f;
}
