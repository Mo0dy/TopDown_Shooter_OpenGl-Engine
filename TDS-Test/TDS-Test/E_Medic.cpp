#include "E_Medic.h"

//Load textures
void E_Medic::Load_E_Medic() {
	ResourceManager::LoadEtex("Textures", "Medic", ".png", GL_TRUE, "Medic", GL_FALSE);
}

//Constructor
E_Medic::E_Medic(glm::vec2 position) : Enemy(position)
{
	//Set texture and hitbox
	etex = ResourceManager::GetEtex("Medic");
	etex.setTexSize(1);
	etex.fitHitboxToTex();
	updateHitboxes();

	//Very low health, one shot will be more than enough to kill
	maxHealth = 40;
	health = maxHealth;

	dynFricCoeff = -2.5;

	damage = 500;
	attackSpeed = 0;
	mass = 7.5;
	movForce = 200;

	turnSpeed = 1;
}


E_Medic::~E_Medic()
{
}

GLboolean E_Medic::updateE(GLfloat dt) 
{
	//Only update if alive
	if (!death) {
		//Check if it died
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}
		setColor(glm::vec3(1 - health / maxHealth, color.y * health / maxHealth, color.z * health / maxHealth));

		//Check if this enemy has attacked, if it has, kill it.
		if (!attacking) {
			
		}
		
		//Decide which player to attack (aka who is closest)
		Player* gPlayer = Game::sPlayers[0];
		for (int i = 0; i < Game::sPlayers.size(); i++) {
			if (glm::distance(pos, gPlayer->pos) > glm::distance(pos, Game::sPlayers[i]->pos)) {
				gPlayer = Game::sPlayers[i];
			}
		}
		
		//Add forces towards the player
		glm::vec2 movDir = gPlayer->pos - pos;

		addForce(glm::normalize(movDir) * movForce);

		setBodyAngle(dt);

		updatePos(dt);

		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}