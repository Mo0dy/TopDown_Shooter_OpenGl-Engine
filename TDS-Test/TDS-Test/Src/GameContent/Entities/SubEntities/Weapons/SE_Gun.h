#pragma once

#include <Windows.h>
#include "..\SubE.h"
#include "..\..\Bullets\Bullet.h"
#include "..\..\CompE.h"

class SE_Gun : public SubE
{
public:
	SE_Gun(CompE* masterE, glm::vec2 rPos, Etex* etex, GLfloat width, GLfloat height, GLfloat shootDelay, GLfloat recoil, GLfloat accuracy);
	SE_Gun(CompE* masterE, glm::vec2 rPos, Etex* etex, glm::vec3 size, GLfloat shootDelay, GLfloat recoil, GLfloat accuracy);
	~SE_Gun();

	void Shoot(std::vector<const Entity*> dmgWhitelist);
	void Reload();

	GLboolean UpdateE(GLfloat dt);

protected:
	GLfloat damage;
	GLfloat magSize;
	GLfloat shootDelay;
	GLfloat recoil;
	GLfloat accuracy;

	std::vector<const Entity*> colWhitelist;

	GLfloat bullets; // the bullets that are currently in the mag
	GLfloat lastShot;
};

