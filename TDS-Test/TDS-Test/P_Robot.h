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

	GLfloat lastShot;
	void shoot();

private:
	glm::vec2 movDir;
	glm::vec2 bodyDir;

	GLfloat shootDelay;
	GLfloat accuracy; // +- shooting angle;

	MOV_STATE movState;

	void setBodyAngle(GLfloat dt);
	void setTrackAngle(GLfloat dt);

	GLfloat bodyTurnSpeed;
	GLfloat trackTurnSpeed;
};

