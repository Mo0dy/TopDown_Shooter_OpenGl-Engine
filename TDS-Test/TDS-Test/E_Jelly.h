#pragma once
#include "Enemy.h"
#include "Util.h"

static const GLfloat JELLY_START_SIZE = 1;
static const GLfloat MIN_JELLY_SIZE = 0.5;
static const GLfloat MAX_INIT_JUMP_VELOCITY = 4;
static const GLfloat MAX_HEALTH = 100;

class E_Jelly : public Enemy
{
public:
	static void Load_E_Jelly();
	E_Jelly(glm::vec2 position, GLfloat size);
	~E_Jelly();

	GLboolean UpdateE(GLfloat dt);
	GLfloat jellySize;
	
	GLfloat lastJump;
	GLfloat jumpTime;
	GLfloat jumping;
	GLfloat initJumpVel;

	GLfloat verticalVel;
	GLfloat height;
protected:
};

