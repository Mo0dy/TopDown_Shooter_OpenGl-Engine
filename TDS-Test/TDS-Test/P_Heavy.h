#pragma once

#include "Player.h"

class P_Heavy : public Player
{
public:
	P_Heavy(glm::vec2 position);
	~P_Heavy();

	GLboolean updateE(GLfloat dt);

};

