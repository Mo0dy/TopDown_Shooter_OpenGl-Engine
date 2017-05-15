#pragma once
#include "LivingE.h"

class Enemy: public LivingE
{
public:
	Enemy(glm::vec2 position);
	~Enemy();

	void ColWithPlayer(class Player* player, GLfloat colDepth, glm::vec2 colAxis);

	GLboolean checkForErase(glm::vec2 levelSize);

	GLfloat lastAttack;
	GLfloat attackSpeed;
	GLboolean attacking;
	GLfloat damage;

	void setBodyAngle(GLfloat dt);
	GLfloat movForce;
	GLfloat turnSpeed;

};

