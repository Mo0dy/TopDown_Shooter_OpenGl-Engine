#include "E_Medic.h"

//Load textures
void E_Medic::Load_E_Medic() {
	ResourceManager::LoadEtex("Textures", "Medic", ".png", GL_TRUE, "Medic", HBOX_AUTOFIT);
}

//Constructor
E_Medic::E_Medic(glm::vec2 position) : Enemy(position)
{
	//Set texture and hitbox
	this->size = glm::vec3(1);
	tex = ResourceManager::GetEtex("Medic").GetTex();
	hitObjs = ResourceManager::GetEtex("Medic").GetHitComb(this->size);

	//Very low health, one shot will be more than enough to kill
	maxHealth = 50;
	health = maxHealth;

	dynFricCoeff = -2.5;

	damage = 400;
	healing = 1000;
	attackSpeed = 1;
	mass = 7.5;
	movForce = 150;

	turnSpeed = 5;

	state = NO_DYN_FRIC;
}


E_Medic::~E_Medic()
{
}

GLboolean E_Medic::UpdateE(GLfloat dt) 
{
	//Only update if alive
	if (!death) {
		//Check if it died
		if (health <= 0) {
			if (attacking) 
			{
				for (int i = 0; i < Game::sPlayers.size(); i++)
				{
					// This should probably be done differently
					Game::sPlayers[i]->GetAttacked(-healing);
				}
			}
			death = GL_TRUE;
			return GL_FALSE;
		}

		//setColor(glm::vec3(1 - health / maxHealth, color.y * health / maxHealth, color.z * health / maxHealth));

		//Check if this enemy has attacked, if it has, kill it.
		if (!attacking) {
			death = GL_TRUE;
			health = 0;
		}
		
		//Decide which player to attack (aka who is closest)
		Player* gPlayer = Game::sPlayers[0];
		for (int i = 0; i < Game::sPlayers.size(); i++) {
			if (glm::distance(this->Get2DPos(), gPlayer->Get2DPos()) > glm::distance(this->Get2DPos(), Game::sPlayers[i]->Get2DPos())) {
				gPlayer = Game::sPlayers[i];
			}
		}
		
		//Add forces towards the player
		glm::vec2 movDir = gPlayer->Get2DPos() - this->Get2DPos();

		AddForce(glm::normalize(movDir) * movForce);

		SetBodyAngle(dt);

		UpdatePos(dt);

		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}