#pragma once

#include "DynE.h"

enum MovementState {
	RUNNING,
	SPRINTING,
	STOPPING
};

class Player : public DynE
{
public:
	Player(std::string texture);	
	~Player();

	void updateE(GLfloat dt);

	float inherentForce;
	float movState;
};

