#pragma once
#include "Player.h"

class Enemy: public DynE
{
public:
	Enemy(glm::vec2 position);
	~Enemy();
	
	GLfloat health;

	void ColWithPlayer(class Player* player, GLfloat colDepth, glm::vec2 colAxis);

	GLboolean checkForErase(glm::vec2 levelSize);

	GLfloat lastAttack;
	GLfloat attackSpeed;
	GLboolean attacking;
	GLfloat damage;
};

