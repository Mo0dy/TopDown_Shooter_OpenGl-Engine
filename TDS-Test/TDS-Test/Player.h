#pragma once

#include "CompE.h"
#include "Bullet.h"
#include <Windows.h>
#include <Xinput.h>

class Player : public CompE
{
public:
	XINPUT_GAMEPAD gPad;

	Player(glm::vec2 position);
	virtual ~Player();

	// Getters and setters:
	GLfloat getInherentF();
	void Collision(Entity* cE, GLfloat dt);

protected:
	GLboolean death;
	GLfloat inherentForce;
	GLfloat health;
};

