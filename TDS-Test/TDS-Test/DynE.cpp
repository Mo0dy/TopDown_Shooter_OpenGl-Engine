#include "DynE.h"

#include <iostream>
#define LOG(x) std::cout << x << std::endl;

DynE::DynE(std::string texture) : Entity(texture), mass(1), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-0.1f)
{
}

DynE::~DynE()
{
}

void DynE::updateE(float dt) {
	vel += dt * (force + fricRes() + airRes() / mass);
	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
}

// Utitlity functions
glm::vec2 DynE::fricRes() {
	if (glm::length(vel) > 0) {
		return vel * dynFricCoeff * mass * GRAV_ACC; // Dynamic Friction
	}
	else {
		return vel * statFricCoeff * mass * GRAV_ACC; // Static Friction
	}
}

glm::vec2 DynE::airRes() {
	return vel * glm::pow(glm::length(vel), 2) * airFricCoeff; // Air resistance
}

// Getters and Setters

void DynE::addForce(glm::vec2 f) {
	force += f;
}
