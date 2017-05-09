#include "AwesomeFace.h"
#include "Game.h"

void AwesomeFace::loadAwesomeface() {
	ResourceManager::LoadTexture("Textures\\awesomeface.png", GL_TRUE, "awesomeface");
}

AwesomeFace::AwesomeFace(glm::vec2 position) : Enemy(position)
{
	tex = "awesomeface";
	health = 100;
	movForce = 110;
	state = MOVING;
	airFricCoeff = -1;

	damage = 800;
	attacking = GL_TRUE;
	attackSpeed = 0.5;
}

AwesomeFace::~AwesomeFace()
{
}

GLboolean AwesomeFace::updateE(GLfloat dt) {
	lastAttack += dt;

	if (!attacking && lastAttack > attackSpeed) {
		attacking = GL_TRUE;
	}

	// updating values according to collision
	if (collision) {
		vel = colVel;
		collision = GL_FALSE;
	}
	addForce(airRes());

	// Pathfinding to player
	addForce(glm::normalize(Game::Players[0]->pos - pos) * movForce);

	 //Unordered group dynamics <-- low fps
	glm::vec2 movDir = glm::normalize(Game::Players[0]->pos - pos) * movForce;
	for(Enemy *e : Game::Enemies) {
		if (glm::distance(pos, e->pos) > 0.4) {
			movDir += glm::normalize(pos - e->pos) * 500.0f /  glm::pow(glm::distance(pos, e->pos), 4);
		}
	}

	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	//if (vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
	//	vel = glm::vec2(0, 0);
	//}
	//else {
		vel += dV;
	//}

	color = glm::vec3(1.0f, health / 100.0f, 1.0f);

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
	return glm::length(vel) > 0;
}

