#include "Player.h"



Player::Player(std::string texture) : DynE(texture)
{
	selfPropForce = 10;
}

Player::~Player()
{
}

// It would be better to just extend the doStep function. 
void Player::updatePlayer(GLfloat dt) {
	doStep(dt);
}