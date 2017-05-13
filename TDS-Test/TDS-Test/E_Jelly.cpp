#include "E_Jelly.h"

#include "Game.h"

void E_Jelly::Load_E_Jelly() {
	ResourceManager::LoadEtex("Textures", "Pudding", ".png", GL_TRUE, "Pudding", GL_TRUE);
}
E_Jelly::E_Jelly(glm::vec2 position, GLfloat size) : Enemy(position)
{
	jellySize = size;

	height = 1;

	etex = ResourceManager::GetEtex("Pudding");
	etex.setTexSize(glm::vec2(size));
	updateHitboxes();

	setColor(glm::vec3(0.05, 1, 0.1));

	maxHealth = 50 * size;
	health = maxHealth;

	dynFricCoeff = -2.5;

	damage = 800 * size;
	attackSpeed = 2;
	mass = 7.5 * size;
	movForce = 150;

	verticalVel = 0;
	turnSpeed = 40;
	jumpTime = 1;
	lastJump = 0;
	jumping = GL_FALSE;
}
E_Jelly::~E_Jelly()
{
}

GLboolean E_Jelly::updateE(GLfloat dt)
{
	if (!death) {
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}
		lastJump += dt;
		setColor(glm::vec3(1 - health / maxHealth, color.y * health / maxHealth, color.z * health / maxHealth));
		lastAttack += dt;
		if (!attacking && lastAttack > attackSpeed) {
			attacking = GL_TRUE;
		}

		verticalVel -= Util::GRAV_ACC * dt;
		height += verticalVel * dt;

		if (height <= 1) {
			height = 1;
			if (jumpTime - 0.2 < lastJump) {
				state = MOVING;
			}
			else {
				state = STOPPING;
			}
			
			if (jumping) {
				jumping = GL_FALSE;
				lastJump = 0;
			}
			else if (jumpTime < lastJump) {
				verticalVel = 5;
				jumping = GL_TRUE;
			}

			Player* gPlayer = Game::Players[0];
			for (int i = 0; i < Game::Players.size(); i++) {
				if (glm::distance(pos, gPlayer->pos) > glm::distance(pos, Game::Players[i]->pos)) {
					gPlayer = Game::Players[i];
				}
			}

			glm::vec2 movDir = gPlayer->pos - pos;

			addForce(glm::normalize(movDir) * movForce);

			setBodyAngle(dt);

		}
		else {
			state = MOVING;
		}

		etex.setTexSize(glm::vec2(jellySize * height));
		updatePos(dt);
		
		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}