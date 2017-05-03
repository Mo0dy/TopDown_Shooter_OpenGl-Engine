#include "DynE.h"

#include <iostream>
#define LOG(x) std::cout << x << std::endl;

DynE::DynE(std::string texture) : Entity(texture), mass(1), force(0, 0), vel(0, 0), selfPropForce(0), movState(STOPPING), fricCoeff(-0.1f)
{
}


DynE::~DynE()
{
}

glm::vec2 DynE::fricRes() {
	GLfloat resMag = glm::pow(glm::length(vel), 2) * fricCoeff; // quadratic resistance function limits max speeds
	if (movState == STOPPING) {
		resMag -= selfPropForce;
	}
	return vel * resMag;
}

void DynE::doStep(float dt) {
	vel += dt * (force / mass + fricRes());
	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
}

// Getters and Setters
GLfloat DynE::getSelfPropForce() { return selfPropForce; }

void DynE::addForce(glm::vec2 f) {
	force += f;
}
