#pragma once

#include "Player.h"
#include "EnergyBullet.h"
#include "SE_BodyPart.h"

class Robot : public Player
{
public:
	enum MOV_STATE {
		AIMING,
		NORMAL,
		SPRINTING
	};

	static void loadRobot();

	Robot(glm::vec2 position);
	~Robot();

	GLboolean UpdateE(GLfloat dt);

	void shoot();
	void shootBigB();

private:

	GLboolean PreUpdate();

	glm::vec2 movDir;
	glm::vec2 bodyDir;

	GLboolean artilMode;
	glm::vec2 bulletSpawn; // in %
	GLfloat lastShot;
	GLfloat lastShotBigB;
	GLfloat shootDelay;
	GLfloat shootDelayBigB;
	GLfloat accuracy; // +- shooting angle;

	MOV_STATE movState;

	void SwitchToArtil();
	void SwitchToNormal();

	void LaserPointer();
};

