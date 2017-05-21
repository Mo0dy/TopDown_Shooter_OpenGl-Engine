#include "E_Jelly.h"

#include "Game.h"

//Load textures
void E_Jelly::Load_E_Jelly() {
	ResourceManager::LoadEtex("Textures", "Pudding", ".png", GL_TRUE, "Pudding", HBOX_AUTOFIT);
}

//Constructor
E_Jelly::E_Jelly(glm::vec2 position, GLfloat size) : Enemy(position)
{
	jellySize = size;
	height = 1;
	this->SetSize(glm::vec2(jellySize));

	//Set texture and hitbox

	tex = ResourceManager::GetEtex("Pudding").GetTex();
	hitObjs = ResourceManager::GetEtex("Pudding").GetHitComb(this->size);

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
	state = NO_DYN_FRIC;
}

//Destructor
E_Jelly::~E_Jelly()
{
}

//Update
GLboolean E_Jelly::UpdateE(GLfloat dt)
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
			state = DYN_FRIC;
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
			else if (jumpTime - 0.1 < lastJump) {
				state = NO_DYN_FRIC;
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
		}
		else {
			state = NO_DYN_FRIC;
		}

		//Larger texture if it's higher
		this->SetSize(glm::vec2(jellySize * height));
		UpdatePos(dt);

		return glm::length(vel) > 0;
	}
	erase = GL_TRUE;
	return GL_FALSE;
}