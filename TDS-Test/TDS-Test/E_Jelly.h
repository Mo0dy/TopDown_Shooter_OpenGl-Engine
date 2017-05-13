#pragma once
#include "Enemy.h"
#include "Util.h"

class E_Jelly : public Enemy
{
public:
	static void Load_E_Jelly();
	E_Jelly(glm::vec2 position, GLfloat size);
	~E_Jelly();

	GLboolean updateE(GLfloat dt);
	GLfloat jellySize;
	
	GLfloat lastJump;
	GLfloat jumpTime;
	GLfloat jumping;

	GLfloat verticalVel;
	GLfloat height;
protected:
};

