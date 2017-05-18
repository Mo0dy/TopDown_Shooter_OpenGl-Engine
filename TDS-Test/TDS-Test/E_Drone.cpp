#include "E_Drone.h"

//Load texture
void E_Drone::load_E_Drone() {
	ResourceManager::LoadAnimation("Textures\\A_Drone", ".png", 31, 1.5, GL_TRUE, "E_Drone_A", ANI_HBOX_AUTOFIT);
}

//Constructor
E_Drone::E_Drone(glm::vec2 position) : Enemy(position)
{
	maxHealth = 70;
	health = maxHealth;
	mass = 10;
	movForce = 120;
	state = NO_DYN_FRIC;
	airFricCoeff = -1;

	damage = 400;
	attackSpeed = 0.5;


	turnSpeed = 10;

	//Set animation
	animations["Flying"] = Animation("E_Drone_A", 1.0, GL_TRUE);
	animations["Flying"].SetAniTime(1);
	animations["Flying"].Start();

	ani = "Flying";

	swarmFactor = 715;
	swarmPot = 2.05;
}

//Destructor
E_Drone::~E_Drone()
{
}

//Update
GLboolean E_Drone::UpdateE(GLfloat dt) {
	//Don't update if it's dead
	if (!death) 
	{
		//Check for death
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}
		lastAttack += dt;
		UpdateAni();

		//Check if attacking is possible
		if (!attacking && lastAttack > attackSpeed) {
			attacking = GL_TRUE;
		}

		//Find the closest player to attack
		Player* gPlayer = Game::sPlayers[0];

		for (int i = 0; i < Game::sPlayers.size(); i++) {
			if (glm::distance(pos, gPlayer->GetPos()) > glm::distance(pos, Game::sPlayers[i]->GetPos())) {
				gPlayer = Game::sPlayers[i];
			}
		}

		glm::vec2 movDir = glm::normalize(gPlayer->GetPos() - pos) * movForce;

		//Drones all hate eachother and try to stay away from other enemies
		for (Enemy *e : Game::sEnemies) {
			if (glm::distance(pos, e->GetPos()) > 0.4) {
				movDir += glm::normalize(pos - e->GetPos()) * swarmFactor / glm::pow(glm::distance(pos, e->GetPos()), 2);
			}
		}

		AddForce(glm::normalize(movDir) * movForce);
		UpdatePos(dt);
		color = glm::vec3(1.0f, health / maxHealth, 1.0f);
		SetBodyAngle(dt);

		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}