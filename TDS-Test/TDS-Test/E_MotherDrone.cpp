#include "E_MotherDrone.h"

E_MotherDrone::E_MotherDrone(glm::vec2 position): E_Drone(position)
{
	for (Etex e : Animations[ani].Etextures) {
		e.setTexSize(5);
	}
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

	if (!attacking && lastAttack > attackSpeed) {
		attacking = GL_TRUE;
	}

	glm::vec2 movDir = glm::vec2(0);

	glm::mat2 rot90DegreesM = Util::rotationMat2(90);

	for(Player *p : Game::Players) {
		movDir += glm::normalize(rot90DegreesM * (pos - p->pos));
		movDir += glm::normalize(p->pos - pos) * glm::pow(glm::distance(p->pos, pos) - 20, 3) * 0.2f;
	}

	addForce(glm::normalize(movDir) * movForce);

	color = glm::vec3(1.0f, health / maxHealth, 1.0f);
	setBodyAngle(dt);
	
	updatePos(dt);
	return glm::length(vel) > 0;
}
