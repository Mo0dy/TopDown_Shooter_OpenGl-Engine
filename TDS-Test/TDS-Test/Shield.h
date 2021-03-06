#pragma once

#include "SubE.h"

class Shield : public SubE
{
public:

	static void LoadShield();

	Shield(class CompE* masterE);
	virtual ~Shield();

	GLboolean UpdateE(GLfloat dt);
	
	void GetAttacked(GLfloat damage);

	void StartShield();
	void StopShield();

	void ToggleShield();

protected:

	void UpdatePos();

	glm::vec2 shieldSize;
	std::vector<HitObject> shieldHObjs;
	GLboolean active;
	GLfloat maxShieldHP;
	GLfloat shieldHP;
	GLfloat rechargePerSec;
};

