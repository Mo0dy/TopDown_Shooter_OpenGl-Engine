#include "E_MotherDrone.h"

//Constructor
E_MotherDrone::E_MotherDrone(glm::vec2 position): E_Drone(position)
{
	animations[ani].enforceWidth(5);
	maxHealth = 4000;
	health = maxHealth;
	movForce = 1500;
	mass = 300;
	turnSpeed = 5;
	airFricCoeff = -10;
}

//Destructor
E_MotherDrone::~E_MotherDrone()
{
}

//Update
GLboolean E_MotherDrone::updateE(GLfloat dt) {
	//Don't update if dead
	if (!death)
	{
		//Check for death
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}
		lastAttack += dt;

		updateAni();

		//Check if attacking is possible
		if (!attacking && lastAttack > attackSpeed) {
			attacking = GL_TRUE;
		}

		glm::vec2 movDir = glm::vec2(0);

		glm::mat2 rot90DegreesM = Util::rotationMat2(90);

		//Please replace this comment for this for loop, I'm just not sure what happens here.
		for (Player *p : Game::sPlayers) {
			movDir += glm::normalize(rot90DegreesM * (pos - p->pos));
			movDir += glm::normalize(p->pos - pos) * glm::pow(glm::distance(p->pos, pos) - 20, 3) * 0.2f;
		}

		addForce(glm::normalize(movDir) * movForce);

		color = glm::vec3(1.0f, health / maxHealth, 1.0f);
		setBodyAngle(dt);

		updatePos(dt);
		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}
