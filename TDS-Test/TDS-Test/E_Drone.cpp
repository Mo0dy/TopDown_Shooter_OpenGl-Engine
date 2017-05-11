#include "E_Drone.h"

void E_Drone::load_E_Drone() {
	Animation::LoadAnimation("Textures\\A_Drone", ".png", 31, 1.5, GL_TRUE, "E_Drone_A");
}

E_Drone::E_Drone(glm::vec2 position) : Enemy(position)
{
	tex = "E_Drone";
	size = glm::vec2(1.5);
	autofitHitbox();
	maxHealth = 70;
	health = maxHealth;
	movForce = 120;
	state = MOVING;
	airFricCoeff = -1;

	damage = 0;
	attacking = GL_TRUE;
	attackSpeed = 0.5;

	turnSpeed = 10;

	ani = new Animation("E_Drone_A", GL_TRUE);
	ani->animationTime = 1;
	ani->startAnimation();

	swarmFactor = 715;
	swarmPot = 2.05;
}

E_Drone::~E_Drone()
{
	delete ani;
}

GLboolean E_Drone::updateE(GLfloat dt) {
	lastAttack += dt;

	if (ani->getState()) {
		tex = ani->getETex()->tex;
		size = ani->getETex()->texSize;
	}

	if (!attacking && lastAttack > attackSpeed) {
		attacking = GL_TRUE;
	}

	// updating values according to collision
	if (collision) {
#ifdef DEBUG_FORCES
		Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
		Renderer::drawLineBuffer.push_back(myVertex((pos + (colVel - vel) * mass / dt * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES
		vel = colVel;
		collision = GL_FALSE;
	}
	addForce(airRes());

	// Pathfinding to player
	//addForce(glm::normalize(Game::Players[0]->pos - pos) * movForce);

	 //Unordered group dynamics <-- low fps
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

	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	//if (vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
	//	vel = glm::vec2(0, 0);
	//}
	//else {
	vel += dV;
	//}

	color = glm::vec3(1.0f, health / maxHealth, 1.0f);
	setBodyAngle(dt);
	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;

#ifdef DEBUG_FORCES
	Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
	Renderer::drawLineBuffer.push_back(myVertex((pos + force * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES
	force = glm::vec2(0, 0);
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