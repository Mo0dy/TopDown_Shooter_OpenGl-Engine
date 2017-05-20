#pragma once

#include "LivingE.h"
#include "Bullet.h"
#include <Windows.h>
#include <Xinput.h>

class Player : public LivingE
{
public:

	Player(glm::vec2 position);
	virtual ~Player();

	void SetGamepad(const XINPUT_GAMEPAD gPad);
	void SetKeys(GLboolean *keys);

protected:
	XINPUT_GAMEPAD gPad;
	GLboolean* keys;
};

