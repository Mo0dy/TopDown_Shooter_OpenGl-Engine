#include "P_Robot.h"

#include "Renderer.h"
#include "Game.h"
#include "EnergyBulletBig.h"

#include <glm\gtc\random.hpp>

void Robot::loadRobot() {
	ResourceManager::LoadTexture("Textures\\D_Bot.png", GL_TRUE, "D_Bot");
	ResourceManager::LoadTexture("Textures\\U_Bot.png", GL_TRUE, "U_Bot");

	Animation::LoadAnimation("Textures\\A_Robot_Shoot", ".png", 2, 1.5, GL_TRUE, "Robot_Shoot");
}

Robot::Robot(glm::vec2 position) : Player(position)
{
	// Settings
	inherentForce = 2500;
	bodyTurnSpeed = 10;
	trackTurnSpeed = 5;
	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;
	accuracy = 0.1;
	health = MAX_HEALTH;

	death = GL_FALSE;

	bulletSpawn = glm::vec2(100, 100);

	subEntities["tracks"] = new SubE(glm::vec2(0));
	subEntities["body"] = new SubE(glm::vec2(0));

	subEntities["tracks"]->tex = "D_Bot";
	subEntities["tracks"]->size = glm::vec2(1.05);

	subEntities["body"]->tex = "U_Bot";
	subEntities["body"]->size = glm::vec2(1.5, 0.85);

	subEntities["tracks"]->autofitHitbox();
	subEntities["body"]->autofitHitbox();

	state = STOPPING;

	shootDelay = 0.1;
	shootDelayBigB = 5;
	setColor(glm::vec3(1.0f));

	renderList.push_back("tracks");
	renderList.push_back("body");

	lastShot = 100;
	lastShotBigB = 100;

	Animations["Robot_Shoot"] = new Animation("Robot_Shoot", GL_FALSE);
}

Robot::~Robot()
{
}

GLboolean Robot::updateE(GLfloat dt) {
	if (!death) {
		if (Animations["Robot_Shoot"]->getState()) {
			subEntities["body"]->tex = Animations["Robot_Shoot"]->getETex()->tex;
			subEntities["body"]->size = Animations["Robot_Shoot"]->getETex()->texSize;
		}

		if (health < 0) {
			death = true;
		}

		setColor(glm::vec3(1 - health / MAX_HEALTH, color.y * health / MAX_HEALTH, color.z * health / MAX_HEALTH));

		movState = NORMAL;
		accuracy = 0.1;
		lastShot += dt;
		lastShotBigB += dt;
		// updating values according to collision
		if (collision) {
#ifdef DEBUG_FORCES
			Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
			Renderer::drawLineBuffer.push_back(myVertex((pos + (colVel - vel) * mass / dt * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES
			vel = colVel;
			collision = GL_FALSE;
			state = MOVING;
		}
		else {
			state = STOPPING;
		}

		force += airRes();

		if (abs(gPad.sThumbLX) > CONTROLLER_DEADZONE || abs(gPad.sThumbLY) > CONTROLLER_DEADZONE) {
			movDir += glm::vec2(gPad.sThumbLX, 0);
			movDir += glm::vec2(0, gPad.sThumbLY);
			state = MOVING;
		}

		if (abs(gPad.sThumbRX) > CONTROLLER_DEADZONE || abs(gPad.sThumbRY) > CONTROLLER_DEADZONE) {
			bodyDir += glm::vec2(gPad.sThumbRX, 0);
			bodyDir += glm::vec2(0, gPad.sThumbRY);
		}

		if (gPad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			movState = AIMING;
		}
		if (gPad.bLeftTrigger > 0) {
			movState = SPRINTING;
		}

		// adding movement direction
		if (glm::length(movDir) > 0) {
			movDir = glm::normalize(movDir);
		}

		if (movState == AIMING) {
			accuracy = 0.01;
			shootDelay = 0.025;
		}
		else {
			shootDelay = 0.1;
		}

		switch (state) {
		case STOPPING: force += fricRes();
			break;
		case MOVING:
			switch (movState) {
			case AIMING:
				force += movDir * inherentForce * 0.1f;
				accuracy = 0.01;
				break;
			case NORMAL:
				force += movDir * inherentForce;
				break;
			case SPRINTING:
				force += movDir * inherentForce * (1.0f + 3.0f / CONTROLLER_TRIGGER_MAX * (GLfloat)gPad.bLeftTrigger);
				accuracy = 0.1 * (1.0f + 3.0f / CONTROLLER_TRIGGER_MAX * (GLfloat)gPad.bLeftTrigger);
				break;
			}
			break;
		}

		if (gPad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			shoot();
		}

		if (gPad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
			shootBigB();
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

		std::cout << "TRACKS = " << angle << " || BODY = " << subEntities["body"]->rAngle << std::endl;

		updateSupE();
		combineHitboxes();

#ifdef DEBUG_FORCES
		Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
		Renderer::drawLineBuffer.push_back(myVertex((pos + force * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES

		force = glm::vec2(0);
		movDir = glm::vec2(0);
		bodyDir = glm::vec2(0);
		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}

void Robot::shoot() {
	if (lastShot > shootDelay) {
		Animations["Robot_Shoot"]->animationTime = shootDelay;
		Animations["Robot_Shoot"]->startAnimation();
		lastShot = 0;
		Game::Bullets.push_back(new EnergyBullet(pos + Util::create2DrotMatrix(subEntities["body"]->angle) * (bulletSpawn * 0.005f), subEntities["body"]->angle + accuracy * (rand() % 2000 / 1000.0f - 1)));
		Game::Bullets.back()->whitelist.push_back(this);
	}
}

void Robot::shootBigB() {
	if (lastShotBigB > shootDelayBigB) {
		lastShotBigB = 0;
		Game::Bullets.push_back(new EnergyBulletBig(pos, subEntities["body"]->angle));
		Game::Bullets.back()->whitelist.push_back(this);
	}
}

void Robot::setBodyAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());
	subEntities["body"]->rAngle = glm::mod<GLfloat>(subEntities["body"]->rAngle, 2 * glm::pi<GLfloat>());
	GLfloat dA = Util::calcMovAngle(subEntities["body"]->rAngle + angle, bodyDir);
	if (abs(dA) > 0) {
		if (bodyTurnSpeed * dt > abs(dA)) {
			subEntities["body"]->rAngle += dA;
		}
		else {
			subEntities["body"]->rAngle += dA / abs(dA) * bodyTurnSpeed * dt;
		}
	}
}

void Robot::setTrackAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());
	GLfloat dA = Util::calcMovAngle(angle, vel);

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
			angle += dA;
		}
		else {
			angle += dA / abs(dA) * trackTurnSpeed * dt;
		}
	}

}