#pragma once

#include "DynE.h"

enum MovementState {
	RUNNING,
	SPRINTING,
	STOPPING
};

enum WeaponState {
	NORMAL,
	AIMING
};

class Player : public DynE
{
public:
	Player(std::string texture, GLfloat height);
	Player(std::string bodyTexture, std::string aimTexture, GLfloat bodyHeight, std::string legTex, GLfloat legHeight);
	~Player();
	GLboolean updateE(GLfloat dt);
	MovementState movState;
	WeaponState wepState;
	glm::vec2 movDir;

	// Getters and setters:
	GLfloat getInherentF();
	Entity* getLegs();

protected:
	std::string standardTex;
	std::string aimTex;
	Entity* legs;
	GLfloat inherentForce;
	GLfloat sprintMod; // inherentForce multiplayer while sprinting
	GLfloat turnSpeed; // degrees per second

	void setLegAngle(GLfloat dt);
};

