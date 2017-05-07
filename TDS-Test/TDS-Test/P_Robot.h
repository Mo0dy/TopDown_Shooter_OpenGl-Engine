#pragma once

#include "Player.h"
#include "EnergyBullet.h"

class Robot : public Player
{
public:

	static const int TRACKS = 0;

	static void loadRobot();

	Robot(glm::vec2 position);
	~Robot();

	GLboolean updateE(GLfloat dt);

	GLfloat lastShoot;
	void shoot();

private:

	GLfloat shootDelay;

	void setBodyAngle(GLfloat dt);
	void setTrackAngle(GLfloat dt);

	GLfloat bodyTurnSpeed;
	GLfloat trackTurnSpeed;
};

