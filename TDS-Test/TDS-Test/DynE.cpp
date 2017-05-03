#include "DynE.h"

#include <iostream>
#define LOG(x) std::cout << x << std::endl;

DynE::DynE(std::string texture) : Entity(texture), mass(1), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(0.1f)
{
}


DynE::~DynE()
{
}

glm::vec2 DynE::fricRes() {
	GLfloat resMag = glm::pow(glm::length(vel), 2) * airFricCoeff; // Air resistance
	GLfloat resMag = dynFricCoeff * mass * GRAV_ACC; // Dynamic Friction
	return vel * resMag;
}

void DynE::updateE(float dt) {
	vel += dt * (force / mass + fricRes());
	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
}

// Getters and Setters

void DynE::addForce(glm::vec2 f) {
	force += f;
}
