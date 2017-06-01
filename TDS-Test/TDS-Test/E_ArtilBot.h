#pragma once

#include "Enemy.h"
#include "SE_BodyPart.h"

class E_ArtilBot : public Enemy
{
public:

	static void Load_E_ArtilBot();

	E_ArtilBot(glm::vec2 position);
	~E_ArtilBot();

	GLboolean UpdateE(GLfloat dt);

protected:
	GLfloat accuracy;
	void Shoot();
	void SetGunAngle(GLfloat dt);
	glm::vec2 gunDir;
	GLfloat gunTurnSpeed;
};

