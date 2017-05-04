#include "DynE.h"

#include <iostream>
#define LOG(x) std::cout << x << std::endl;

DynE::DynE(std::string texture) : Entity(texture), mass(1), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-0.1f)
{
}

DynE::~DynE()
{
}

GLboolean DynE::updateE(float dt) {
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
