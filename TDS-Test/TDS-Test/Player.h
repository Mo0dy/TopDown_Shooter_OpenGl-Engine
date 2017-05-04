#pragma once

#include "DynE.h"

enum MovementState {
	RUNNING,
	SPRINTING,
	STOPPING
};

class Player : public DynE
{
public:
	Player(std::string texture);	
	~Player();
	void updateE(GLfloat dt);
	MovementState movState;
	glm::vec2 movDir;
	// Getters and setters:
	GLfloat getInherentF();
protected:
	GLfloat inherentForce;
	GLfloat sprintMod; // inherentForce multiplayer while sprinting
	GLfloat turnSpeed; // degrees per second

	void calcAngle(GLfloat dt);
};

