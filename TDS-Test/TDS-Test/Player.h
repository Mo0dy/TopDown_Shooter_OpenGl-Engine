#pragma once

#include "LivingE.h"
#include "Bullet.h"
#include <Windows.h>
#include <Xinput.h>

class Player : public LivingE
{
public:
	XINPUT_GAMEPAD gPad;
	const GLboolean* keys[1024];

	Player(glm::vec2 position);
	virtual ~Player();

	// Getters and setters:
	GLfloat GetInherentF();

protected:
	GLfloat inherentForce;
};

