#pragma once

#include "Enemy.h"
#include "Game.h"
#include "Util.h"

class E_Medic : public Enemy
{
public:
	static void Load_E_Medic();
	E_Medic(glm::vec2 position);
	~E_Medic();
	GLboolean updateE(GLfloat dt);
};

