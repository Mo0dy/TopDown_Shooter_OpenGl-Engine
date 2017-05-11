#include "DynE.h"

#include <iostream>
#include "Renderer.h"

DynE::DynE(glm::vec2 position) : Entity(position), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-1.0f), state(STOPPING), collision(GL_FALSE)
{
}
DynE::DynE(glm::vec2 position, glm::vec2 size) : Entity(position, size), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-1.0f), state(STOPPING), collision(GL_FALSE) {
}
DynE::DynE(glm::vec2 position, glm::vec2 size, GLfloat angle) : Entity(position, size, angle), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-1.0f), state(STOPPING), collision(GL_FALSE) {
}
DynE::DynE(glm::vec2 position, glm::vec2 size, std::string texture) : Entity(position, size, texture), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-1.0f), state(STOPPING), collision(GL_FALSE) {

}
DynE::DynE(glm::vec2 position, glm::vec2 size, GLfloat angle, std::string texture) : Entity(position, size, angle, texture), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), state(STOPPING), collision(GL_FALSE) {

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

void DynE::ColWithStat(Entity *cE, GLfloat colDepth, glm::vec2 minColAxis) {
	collision = GL_TRUE;
	glm::vec2 n = glm::normalize(minColAxis);
	//if (colDepth < LINEAR_COL_FORCE_INTERVAL) {
		pos += -n * colDepth;
	//}
	//else {
	//	addForce(-n * colDepth * MAC_COL_FORCE);
	//}
	colVel = vel;
}

void DynE::ColWithDyn(DynE *cE, GLfloat colDepth, glm::vec2 minColAxis) {
	collision = GL_TRUE;
	glm::vec2 n = glm::normalize(minColAxis);

	GLfloat v1p = glm::dot(vel, n);
	GLfloat v2p = glm::dot(cE->vel, n);

	colVel = vel;

	//if ((v1p > 0 && v2p < v1p) || (v1p < 0 && v2p > v1p)) {
		// colVel = vel + 2 * ((cE->mass * v2p - cE->mass * v1p) / (mass + cE->mass)) * n; // <-- elastic collision
		colVel = vel - cE->mass * COEFFFICIENT_OF_RESTITUTION * (v1p - v2p) / (mass + cE->mass) * n; // <-- partly inelastiv collision
	//}
	pos += -n * 0.5f * colDepth; // Push out of the collision depth
	//if (pos == cE->pos) {
	//	pos += 0.0001;
	//}
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
void DynE::addForce(glm::vec2 f) { force += f; }
glm::vec2 DynE::getVel() { return vel;  }
GLfloat DynE::getAbsVel() { return glm::length(vel);  }
GLfloat DynE::getMass() { return mass; }