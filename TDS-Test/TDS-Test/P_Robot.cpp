#include "P_Robot.h"

#include "Renderer.h"
#include "Game.h"

void Robot::loadRobot() {
	ResourceManager::LoadTexture("Textures\\D_Bot.png", GL_TRUE, "D_Bot");
	ResourceManager::LoadTexture("Textures\\U_Bot.png", GL_TRUE, "U_Bot");
	ResourceManager::LoadTexture("Textures\\Tracks.png", GL_TRUE, "Tracks");
}

Robot::Robot(glm::vec2 position): Player(position) 
{
	// Settings
	inherentForce = 2500;
	sprintMod = 4;
	bodyTurnSpeed = 6;
	trackTurnSpeed = 5;
	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;

	tex = "U_Bot";
	size = glm::vec2(1.5);

	addEntities.push_back(new Entity(pos));
	addEntities[TRACKS]->tex = "D_Bot";
	addEntities[TRACKS]->size = glm::vec2(1.5);
	state = STOPPING;

	shootDelay = 0.05;
	setColor(glm::vec3(1.0f));
}

Robot::~Robot()
{
}

GLboolean Robot::updateE(GLfloat dt) {
	// updating values according to collision
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

	lastShoot += dt;

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

	setTrackAngle(dt);
	setBodyAngle(dt);

	addEntities[Robot::TRACKS]->pos = pos;

#ifdef DEBUG_FORCES
	Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
	Renderer::drawLineBuffer.push_back(myVertex((pos + force * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES

	force = glm::vec2(0);
	movDir = glm::vec2(0);
	bodyDir = glm::vec2(0);
	return glm::length(vel) > 0;
}

void Robot::shoot() {
	if (lastShoot > shootDelay) {
		lastShoot = 0;
		Bullets.push_back(new EnergyBullet(pos, angle));
		Bullets.back()->whitelist.push_back(this);
		for (Entity *e : addEntities) {
			Bullets.back()->whitelist.push_back(e);
		}
		for (Entity *e : Bullets) {
			Bullets.back()->whitelist.push_back(e);
		}
	}
}

void Robot::setBodyAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());
	GLfloat dA = Util::calcMovAngle(angle, bodyDir);
	if (abs(dA) > 0) {
		if (bodyTurnSpeed * dt > abs(dA)) {
			angle += dA;
		}
		else {
			angle += dA / abs(dA) * bodyTurnSpeed * dt;
		}
	}
}

void Robot::setTrackAngle(GLfloat dt) {
	addEntities[TRACKS]->angle = glm::mod<GLfloat>(addEntities[TRACKS]->angle, 2 * glm::pi<GLfloat>());
	GLfloat dA = Util::calcMovAngle(addEntities[TRACKS]->angle, vel);

	if (abs(dA) > 0.5 * glm::pi<GLfloat>()) {
		if (dA > 0) {
			dA = dA - glm::pi<GLfloat>();
		}
		else {
			dA = dA + glm::pi<GLfloat>();
		}
	}

	if (abs(dA) > 0) {
		if (trackTurnSpeed * dt > abs(dA)) {
			addEntities[TRACKS]->angle += dA;
		}
		else {
			addEntities[TRACKS]->angle += dA / abs(dA) * trackTurnSpeed * dt;
		}
	}

}