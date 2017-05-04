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
	Player(std::string texture, glm::vec2 s);
	Player(std::string bodyTex, glm::vec2 bodyS, std::string legTex, glm::vec2 legS);
	~Player();
	GLboolean updateE(GLfloat dt);
	MovementState movState;
	glm::vec2 movDir;

	// Getters and setters:
	GLfloat getInherentF();
	Entity* getLegs();

protected:
	Entity* legs;
	GLfloat inherentForce;
	GLfloat sprintMod; // inherentForce multiplayer while sprinting
	GLfloat turnSpeed; // degrees per second

	void setLegAngle(GLfloat dt);
};

