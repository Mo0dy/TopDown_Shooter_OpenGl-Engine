#include "E_Jelly.h"

#include "Game.h"

//Load textures
void E_Jelly::Load_E_Jelly() {
	ResourceManager::LoadEtex("Textures", "Pudding", ".png", GL_TRUE, "Pudding", GL_TRUE);
}

//Constructor
E_Jelly::E_Jelly(glm::vec2 position, GLfloat size) : Enemy(position)
{
	jellySize = size;
	height = 1;

	//Set texture and hitbox
	etex = ResourceManager::GetEtex("Pudding");
	etex.setTexSize(glm::vec2(size));
	updateHitboxes();
	dynFricCoeff = -2.5;

	//Set color to RGB 13, 255, 26.
	SetColor(glm::vec3(0, 1, 0.2));

	//Health is higher for a bigger jelly.
	maxHealth = MAX_HEALTH * glm::pow(size / JELLY_START_SIZE, 2);
	health = maxHealth;

	//Damage and Mass are also higher if the jelly is bigger.
	damage = 800 * glm::pow(size / JELLY_START_SIZE, 2);
	attackSpeed = 2;
	mass = 100 * glm::pow(size / JELLY_START_SIZE, 2);
	movForce = 1000 * glm::pow(size / JELLY_START_SIZE, 2);
	initJumpVel = MAX_INIT_JUMP_VELOCITY - MAX_INIT_JUMP_VELOCITY * 0.5f * glm::pow(size / JELLY_START_SIZE, 2);

	verticalVel = 0;
	turnSpeed = 40;
	jumpTime = 1;
	lastJump = 0;
	jumping = GL_FALSE;
	state = MOVING;
}

//Destructor
E_Jelly::~E_Jelly()
{
}

//Update
GLboolean E_Jelly::updateE(GLfloat dt)
{
	//Only update if alive
	if (!death) {
		//Check if it died
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}
		lastJump += dt;
		SetColor(glm::vec3(1 - health / maxHealth, color.y * health / maxHealth, color.z * health / maxHealth));

		//Check if attack is possible
		lastAttack += dt;
		if (!attacking && lastAttack > attackSpeed) {
			attacking = GL_TRUE;
		}

		verticalVel -= Util::GRAV_ACC * dt;
		height += verticalVel * dt;

		//Do stuff regarding jumping (I'm not sure how this works exactly)
		if (height <= 1) {
			state = STOPPING;
			height = 1;
			if (jumping) {
				jumping = GL_FALSE;
				lastJump = 0;
			}
			else if (jumpTime < lastJump) {
				verticalVel = initJumpVel;
				vel = glm::normalize(vel) * initJumpVel * 4.0f;
				jumping = GL_TRUE;
			}
			else if(jumpTime - 0.1 < lastJump) {
				state = MOVING;
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
		}
		else {
			state = MOVING;
		}

		//Larger texture if it's higher
		etex.setTexSize(glm::vec2(jellySize * height));
		updatePos(dt);
		
		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}