#pragma once

#include "Player.h"
#include "EnergyBullet.h"

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

	GLboolean updateE(GLfloat dt);

	void shoot();
	void shootBigB();

private:
	const GLfloat MAX_HEALTH = 2000;

	std::map<std::string, Animation*> Animations;

	glm::vec2 movDir;
	glm::vec2 bodyDir;

	glm::vec2 bulletSpawn; // in %
	GLfloat lastShot;
	GLfloat lastShotBigB;
	GLfloat shootDelay;
	GLfloat shootDelayBigB;
	GLfloat accuracy; // +- shooting angle;

	MOV_STATE movState;

	void setBodyAngle(GLfloat dt);
	void setTrackAngle(GLfloat dt);

	GLfloat bodyTurnSpeed;
	GLfloat trackTurnSpeed;
};

