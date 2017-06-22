#pragma once
#include "..\LivingE.h"

class Enemy: public LivingE
{
public:
	Enemy(glm::vec2 position);
	~Enemy();

	void ColWithPSubE(class SubE* e, GLfloat colDepth, glm::vec2 colAxis);

	GLboolean CheckForErase(glm::vec2 levelSize);

	GLfloat lastAttack;
	GLfloat attackSpeed;
	GLboolean attacking;
	GLfloat damage;

	GLfloat movForce;
};

