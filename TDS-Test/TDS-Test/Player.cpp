#include "Player.h"

Player::Player(std::string texture) : DynE(texture), inherentForce(1000), movState(STOPPING)
{
	mass = 80;
	airFricCoeff = -5; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;
	size = glm::vec2(1.0f, 0.8f);
}

Player::~Player()
{
}

// It would be better to just extend the doStep function. 
void Player::updateE(GLfloat dt) {
	force += airRes();
	if (movState == STOPPING) {
		force += fricRes();
	}
	vel += dt * (force / mass); // acceleration is in m/s^2 so we need to divide my dt to get a velocity
	LOG(glm::length(vel));
	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
}