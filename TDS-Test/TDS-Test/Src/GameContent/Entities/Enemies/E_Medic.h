#pragma once

#include "Enemy.h"

class E_Medic : public Enemy
{
public:

	static void Load_E_Medic();

	E_Medic(glm::vec2 position);
	~E_Medic();
	GLboolean UpdateE(GLfloat dt);

	GLfloat healing;
};

