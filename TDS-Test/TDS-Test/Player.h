#pragma once

#include "DynE.h"

enum MovementState {
	RUNNING,
	SPRINTING,
};

enum WeaponState {
	NORMAL,
	AIMING
};

class Player : public DynE
{
public:
	Player(glm::vec2 position);
	virtual ~Player();

	MovementState movState;
	WeaponState wepState;
	glm::vec2 movDir;

	// Getters and setters:
	GLfloat getInherentF();
	
	std::vector<Entity*> getAddEntities();
	//std::vector<myAnimation*> animations;
protected:
	std::vector<Entity*> addEntities;
	GLfloat inherentForce;
	GLfloat sprintMod; // inherentForce multiplayer while sprinting
	GLfloat turnSpeed; // degrees per second
};

