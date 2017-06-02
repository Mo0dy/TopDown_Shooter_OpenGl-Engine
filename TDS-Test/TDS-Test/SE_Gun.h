#pragma once

#include "SubE.h"
#include "Bullet.h"

class SE_Gun : public SubE
{
public:
	SE_Gun(class CompE* masterE, glm::vec2 rPos, Bullet bullet, GLfloat shootDelay, GLfloat recoil);
	~SE_Gun();

	void Shoot();
	void Reload();

protected:
	GLfloat damage;
	GLfloat magSize;
	GLfloat shootDelay;
	GLfloat recoil;
	Bullet bullet;

	GLfloat bullets; // the bullets that are currently in the mag
	GLfloat lastShot;
};

