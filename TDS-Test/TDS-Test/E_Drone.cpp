#include "E_Drone.h"

void E_Drone::load_E_Drone() {
	ResourceManager::LoadAnimation("Textures\\A_Drone", ".png", 31, 1.5, GL_TRUE, "E_Drone_A", HBOX_AUTOFIT);
}

E_Drone::E_Drone(glm::vec2 position) : Enemy(position)
{
	maxHealth = 70;
	health = maxHealth;
	movForce = 120;
	state = MOVING;
	airFricCoeff = -1;

	damage = 0;
	attacking = GL_TRUE;
	attackSpeed = 0.5;

	turnSpeed = 10;

	Animations["Flying"] = Animation("E_Drone_A", 1.0, GL_TRUE);
	Animations["Flying"].animationTime = 1;
	Animations["Flying"].startAnimation();

	ani = "Flying";

	swarmFactor = 715;
	swarmPot = 2.05;
}

E_Drone::~E_Drone()
{
}

GLboolean E_Drone::updateE(GLfloat dt) {
	lastAttack += dt;
	updateAni();

	if (!attacking && lastAttack > attackSpeed) {
		attacking = GL_TRUE;
	}

	Player* gPlayer = Game::Players[0];

	for (int i = 0; i < Game::Players.size(); i++) {
		if (glm::distance(pos, gPlayer->pos) > glm::distance(pos, Game::Players[i]->pos)) {
			gPlayer = Game::Players[i];
		}
	}

	glm::vec2 movDir = glm::normalize(gPlayer->pos - pos) * movForce;
	for (Enemy *e : Game::Enemies) {
		if (glm::distance(pos, e->pos) > 0.4) {
			movDir += glm::normalize(pos - e->pos) * swarmFactor / glm::pow(glm::distance(pos, e->pos), 2);
		}
	}

	addForce(glm::normalize(movDir) * movForce);
	updatePos(dt);
	color = glm::vec3(1.0f, health / maxHealth, 1.0f);
	setBodyAngle(dt);

	return glm::length(vel) > 0;
}

void E_Drone::setBodyAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());
	GLfloat dA = Util::calcMovAngle(angle, vel);
	if (abs(dA) > 0) {
		if (turnSpeed * dt > abs(dA)) {
			angle += dA;
		}
		else {
			angle += dA / abs(dA) * turnSpeed * dt;
		}
	}
}