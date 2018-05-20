#pragma once

#include "SubE.h"
#include "Bullet.h"

class SE_Gun : public SubE
{
public:
	SE_Gun(class CompE* masterE, glm::vec2 rPos, Etex* etex, GLfloat width, GLfloat height, Bullet bullet, GLfloat shootDelay, GLfloat recoil, GLfloat accuracy, const std::vector<Enemy*> &damageWhitelist);
	~SE_Gun();

	void Shoot();
	void Reload();

protected:
	GLfloat damage;
	GLfloat magSize;
	GLfloat shootDelay;
	GLfloat recoil;
	GLfloat accuracy;
	Bullet bullet;

	std::vector<const Entity*> damageWhitelist;
	std::vector<const Entity*> colWhitelist;

	GLfloat bullets; // the bullets that are currently in the mag
	GLfloat lastShot;
};

