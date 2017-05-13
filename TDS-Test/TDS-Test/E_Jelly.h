#pragma once
#include "Enemy.h"

class E_Jelly : public Enemy
{
public:
	static void Load_E_Jelly();
	E_Jelly(glm::vec2 position, GLfloat size);
	~E_Jelly();

	GLboolean updateE(GLfloat dt);
	GLfloat jellySize;

protected:
};

