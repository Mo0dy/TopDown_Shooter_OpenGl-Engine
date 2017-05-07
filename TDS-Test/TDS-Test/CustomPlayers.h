#pragma once

#include "Player.h"

class Robot : public Player
{
public:

	static const int TRACKS = 0;

	Robot(glm::vec2 position);
	~Robot();

	GLboolean updateE(GLfloat dt);

private:
	void setBodyAngle(GLfloat dt);
	void setTrackAngle(GLfloat dt);

	GLfloat bodyTurnSpeed;
	GLfloat trackTurnSpeed;
};

