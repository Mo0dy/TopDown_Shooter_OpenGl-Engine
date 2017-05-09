#include "DynE.h"

#include <iostream>
#include "Renderer.h"

DynE::DynE(glm::vec2 position) : Entity(position), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-1.0f), state(STOPPING), collision(GL_FALSE)
{
}

DynE::~DynE()
{
}

GLboolean DynE::updateE(GLfloat dt) {
	// updating values according to collision
	if (collision) {
		vel = colVel;
		collision = GL_FALSE;
	}

	switch (state) {
	case MOVING: addForce(airRes());
		break;
	case STOPPING: addForce(fricRes() + airRes());
	}
	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	if (vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
		vel = glm::vec2(0, 0);
	}
	else {
		vel += dV;
	}

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
	return glm::length(vel) > 0;
}

void DynE::ColWithStat(Entity *cE) {

}

void DynE::ColWithDyn(DynE *cE, GLfloat colDepth) {
	collision = GL_TRUE;
	glm::vec2 c = cE->pos - pos;

	GLfloat v1p = glm::dot(vel, c);
	GLfloat v2p = glm::dot(cE->vel, c);

	if ((v1p > 0 && v2p < v1p) || (v1p < 0 && v2p > v1p)) {
		colVel = vel + 2 * ((cE->mass * glm::dot(cE->vel, c) - cE->mass * glm::dot(vel, c)) / glm::pow(glm::length(c), 2) / (mass + cE->mass)) * c;
	}
	else {
		colVel = vel;
	}
	if (pos == cE->pos) {
		pos += 0.0001;
	}
	addForce(-c * colDepth * 5000.0f);
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
	return glm::vec2(0, 0);
}

// Getters and Setters

void DynE::addForce(glm::vec2 f) {
	force += f;
}