#include "P_Heavy.h"



P_Heavy::P_Heavy(glm::vec2 position): Player(position)
{
	// Settings
	inherentForce = 2500;
	sprintMod = 4;
	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;

	tex = "U_Bot";
	size = glm::vec2(1.5);

	state = STOPPING;

	setColor(glm::vec3(1.0f));
}


P_Heavy::~P_Heavy()
{
}


GLboolean P_Heavy::updateE(GLfloat dt) {
	if (collision) {
		pos = colPos;
		vel = colVel;
		collision = GL_FALSE;
	}

	// We need to remove all bullets that travel too far!!!
	for (int i = 0; i < Bullets.size(); i++) {
		if (Bullets[i]->collision) {
			delete Bullets[i];
			Bullets.erase(Bullets.begin() + i);
		}
		else {
			Bullets[i]->updateE(dt);
		}
	}

	// updating animation
	if (ani.getState()) {
		tex = ani.getETex()->tex;
		size = ani.getETex()->texSize;
		if (ani.getETex()->hitboxes.size() > 0) {
			Hitboxes = ani.getETex()->hitboxes;
		}
	}

	force += airRes();

	// normalizing movDir
	if (glm::length(movDir) > 0) {
		movDir = glm::normalize(movDir);
	}

	switch (state) {
	case STOPPING: force += fricRes();
		break;
	case MOVING: force += movDir * inherentForce;
		break;
	}

	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	if (glm::length(vel) > 0 && vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
		vel = glm::vec2(0, 0);
	}
	else {
		vel += dV; // acceleration is in m/s^2 so we need to divide my dt to get a velocity
	}

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;


#ifdef DEBUG_FORCES
	Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
	Renderer::drawLineBuffer.push_back(myVertex((pos + force * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES

	force = glm::vec2(0);
	movDir = glm::vec2(0);
	bodyDir = glm::vec2(0);
	return glm::length(vel) > 0;
}