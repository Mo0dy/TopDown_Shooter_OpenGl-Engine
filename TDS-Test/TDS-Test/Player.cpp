#include "Player.h"

Player::Player(std::string texture) : DynE(texture), inherentForce(2500), movState(STOPPING), sprintMod(4)
{
	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;
	size = glm::vec2(0.867f, 1.0f);
}

Player::~Player()
{
}

// It would be better to just extend the doStep function.
void Player::updateE(GLfloat dt) {
	force += airRes();

	if (glm::length(movDir) > 0) {
		movDir = glm::normalize(movDir);
	}

	switch(movState) {
	case STOPPING: force += fricRes();
		break;
	case RUNNING: force += movDir * inherentForce;
		break;
	case SPRINTING: force += movDir * inherentForce * sprintMod;
		break;
	}
	 
	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	if (glm::length(vel) > 0 && vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
		vel = glm::vec2(0, 0);
	} else {
		vel += dV; // acceleration is in m/s^2 so we need to divide my dt to get a velocity
	}

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
	movDir = glm::vec2(0, 0);
}

// Getters and setters
GLfloat Player::getInherentF() {
	return inherentForce;
}