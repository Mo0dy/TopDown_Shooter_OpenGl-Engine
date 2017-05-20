#include "Player.h"

Player::Player(glm::vec2 position) : LivingE(position) {
}

Player::~Player()
{
}

void Player::SetGamepad(const XINPUT_GAMEPAD gPad) { this->gPad = gPad; }
void Player::SetKeys(GLboolean* keys) { this->keys = keys; }