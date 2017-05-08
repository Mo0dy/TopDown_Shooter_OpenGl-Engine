#include "AwesomeFace.h"
#include "Game.h"

void AwesomeFace::LoadAwesomeface() {
	ResourceManager::LoadTexture("Textures\\awesomeface.png", GL_TRUE, "awesomeface");
}

AwesomeFace::AwesomeFace(glm::vec2 position) : Enemy(position)
{
	tex = "awesomeface";
	health = 100;
	movForce = 100;
	state = MOVING;
	airFricCoeff = -1;
}

AwesomeFace::~AwesomeFace()
{
}

GLboolean AwesomeFace::updateE(GLfloat dt) {
	// updating values according to collision
	if (collision) {
		pos = colPos;
		vel = colVel;
		collision = GL_FALSE;
	}

	// updating animation
	if (ani.getState()) {
		tex = ani.getETex()->tex;
		size = ani.getETex()->texSize;
		if (ani.getETex()->hitboxes.size() > 0) {
			Hitboxes = ani.getETex()->hitboxes;
		}
	}

	addForce(airRes());

	// Pathfinding to player
	addForce(glm::normalize(Game::Players[0]->pos - pos) * movForce);

	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	if (vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
		vel = glm::vec2(0, 0);
	}
	else {
		vel += dV;
	}

	color = glm::vec3(1.0f, health / 100.0f, 1.0f);

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;
	force = glm::vec2(0, 0);
	return glm::length(vel) > 0;
}

