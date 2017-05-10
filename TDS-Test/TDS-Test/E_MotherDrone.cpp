#include "E_MotherDrone.h"

E_MotherDrone::E_MotherDrone(glm::vec2 position): E_Drone(position)
{
	size = glm::vec2(5);
	Hitboxes.back()->size = size;
	Hitboxes.back()->pos = -0.5f * size;
	maxHealth = 10000;
	health = maxHealth;
	movForce = 1500;
	mass = 300;
	turnSpeed = 5;
	airFricCoeff = -10;
}


E_MotherDrone::~E_MotherDrone()
{
}

GLboolean E_MotherDrone::updateE(GLfloat dt) {
	lastAttack += dt;

	if (ani->getState()) {
		tex = ani->getETex()->tex;
	}

	if (!attacking && lastAttack > attackSpeed) {
		attacking = GL_TRUE;
	}

	// updating values according to collision
	if (collision) {
		vel = colVel;
		collision = GL_FALSE;
	}
	addForce(airRes());

	glm::vec2 movDir = glm::vec2(0);

	glm::mat2 rot90DegreesM = Util::create2DrotMatrix(90);

	for(Player *p : Game::Players) {
		movDir += glm::normalize(rot90DegreesM * (pos - p->pos));
		movDir += glm::normalize(p->pos - pos) * glm::pow(glm::distance(p->pos, pos) - 20, 3) * 0.2f;
	}

	addForce(glm::normalize(movDir) * movForce);

	glm::vec2 dV = dt * force / mass;
	vel += dV;

	color = glm::vec3(1.0f, health / maxHealth, 1.0f);
	setBodyAngle(dt);
	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
	return glm::length(vel) > 0;
}
