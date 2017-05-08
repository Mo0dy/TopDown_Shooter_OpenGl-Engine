#pragma once

#include "Player.h"
#include "EnergyBullet.h"

class Robot : public Player
{
public:
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

	void setBodyAngle(GLfloat dt);
	void setTrackAngle(GLfloat dt);

	GLfloat bodyTurnSpeed;
	GLfloat trackTurnSpeed;
};

