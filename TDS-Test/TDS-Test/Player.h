#pragma once

#include "DynE.h"

class Player : public DynE
{
public:
	Player(std::string texture);	
	~Player();

	void updatePlayer(GLfloat dt);

};

