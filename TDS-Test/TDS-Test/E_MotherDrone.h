#pragma once

#include "E_Drone.h"

class E_MotherDrone: public E_Drone
{
public:

	E_MotherDrone(glm::vec2 position);
	~E_MotherDrone();

	GLboolean updateE(GLfloat dt);
	//void updateAni();

	GLfloat lastSpawn;
	GLboolean spawning;
	GLfloat spawnSpeed;
};

