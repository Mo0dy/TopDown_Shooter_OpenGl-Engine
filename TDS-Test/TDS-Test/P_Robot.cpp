#include "P_Robot.h"

#include "Renderer.h"
#include "Game.h"
#include "EnergyBulletBig.h"
#include "Shield.h"
#include <glm\gtc\random.hpp>

void Robot::loadRobot() {
	ResourceManager::LoadEtex("Textures", "D_Bot", ".png", GL_TRUE, "D_Bot", HBOX_AUTOFIT);
	ResourceManager::LoadEtex("Textures", "U_Bot", ".png", GL_TRUE, "U_Bot", HBOX_AUTOFIT);

	ResourceManager::LoadAnimation("Textures\\A_Robot_Shoot", ".png", 2, 1.5, GL_TRUE, "Robot_Shoot", ANI_LOAD_ONE_HBOX);
	Shield::LoadShield();
}

Robot::Robot(glm::vec2 position) : Player(position)
{
	// Settings
	inherentForce = 2500;
	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;
	accuracy = 0.1;
	maxHealth = 2000;
	health = maxHealth;

	death = GL_FALSE;

	bulletSpawn = glm::vec2(1, 1);

	subEntities["tracks"] = new SE_BodyPart(this, glm::vec2(0), &ResourceManager::GetEtex("D_Bot"), 1.3, 1);
	subEntities["body"] = new SE_BodyPart(this, glm::vec2(0), &ResourceManager::GetEtex("U_Bot"), 2, 1);
	subEntities["shield"] = new Shield(this);

	//subEntities["body"]->animations["ShootSmallB"] = Animation("Robot_Shoot", GL_FALSE);
	
	bodyTurnSpeed = 10;
	trackTurnSpeed = 5;

	state = DYN_FRIC;

	shootDelay = 0.1;
	shootDelayBigB = 5;
	SetColor(glm::vec3(1.0f));

	renderOrder.push_back("shield");
	renderOrder.push_back("tracks");
	renderOrder.push_back("body");

	lastShot = 100;
	lastShotBigB = 100;

	artilMode = GL_FALSE;
}

Robot::~Robot()
{
}

static GLboolean sPressAFlag = GL_FALSE;
static GLboolean sPressXFlag = GL_FALSE;

GLboolean Robot::UpdateE(GLfloat dt) {
	if (!death) {
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}
		lastShot += dt;
		lastShotBigB += dt;
		SetColor(glm::vec3(1 - health / maxHealth, color.y * health / maxHealth, color.z * health / maxHealth));
		movState = NORMAL;
		state = DYN_FRIC;

		// Updating body direction
		if (abs(gPad.sThumbRX) > Util::CONTROLLER_DEADZONE || abs(gPad.sThumbRY) > Util::CONTROLLER_DEADZONE) {
			bodyDir += glm::vec2(gPad.sThumbRX, 0);
			bodyDir += glm::vec2(0, gPad.sThumbRY);
		}

		// toggeling shield
		if (gPad.wButtons & XINPUT_GAMEPAD_A) {
			sPressAFlag = GL_TRUE;
		}
		else if(sPressAFlag) {
			dynamic_cast<Shield*>(subEntities["shield"])->ToggleShield();
			sPressAFlag = GL_FALSE;
		}

		// Switching between artil and normal mode
		if (gPad.wButtons & XINPUT_GAMEPAD_X) {
			sPressXFlag = GL_TRUE;
		}
		else if (sPressXFlag) {
			if (artilMode) {
				SwitchToNormal();
			}
			else {
				SwitchToArtil();
			}
			sPressXFlag = GL_FALSE;
		}

		if (artilMode) { // Artilelry Mode
			if (gPad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				shoot();
			}
		}
		else { // Normal Mode
			if (abs(gPad.sThumbLX) > Util::CONTROLLER_DEADZONE || abs(gPad.sThumbLY) > Util::CONTROLLER_DEADZONE) {
				movDir += glm::vec2(gPad.sThumbLX, 0);
				movDir += glm::vec2(0, gPad.sThumbLY);
				state = NO_DYN_FRIC;
			}
			if (gPad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				movState = AIMING;
			}
			if (gPad.bLeftTrigger > 0) {
				movState = SPRINTING;
			}
			if (glm::length(movDir) > 0) {
				movDir = glm::normalize(movDir);
			}
			switch (movState) {
			case AIMING:
				this->AddForce(movDir * inherentForce * 0.1f);
				accuracy = 0.01;
				break;
			case NORMAL:
				this->AddForce(movDir * inherentForce);
				accuracy = 0.1;
				break;
			case SPRINTING:
				this->AddForce(movDir * inherentForce * (1.0f + 3.0f / Util::CONTROLLER_TRIGGER_MAX * (GLfloat)gPad.bLeftTrigger));
				accuracy = 0.1 * (1.0f + 3.0f / Util::CONTROLLER_TRIGGER_MAX * (GLfloat)gPad.bLeftTrigger);
				break;
			}
			if (gPad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
				shoot();
			}
			if (gPad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
				shootBigB();
			}
			SetTrackAngle(dt);
		}

		SetBodyAngle(dt);
		UpdatePos(dt);
		UpdateSubE(dt);
		movDir = glm::vec2(0);
		bodyDir = glm::vec2(0);
		return glm::length(vel) > 0;
	}
	return GL_FALSE;
}

void Robot::shoot() {
	if (lastShot > shootDelay) {
		//subEntities["body"]->animations["ShootSmallB"].aniTime = shootDelay;
		//subEntities["body"]->animations["ShootSmallB"].Start();
		//subEntities["body"]->ani = "ShootSmallB";
		lastShot = 0;

		// This is probably not the best option to whitelist all players
		std::vector<const Entity*> whitelist;
		for (Player *p : Game::sPlayers) {
			whitelist.push_back(p);
		}
		Game::sBullets.push_back(new EnergyBullet(this->Get2DPos() + Util::RotationMat2(subEntities["body"]->GetAngle()) * bulletSpawn, subEntities["body"]->GetAngle() + accuracy * (rand() % 2000 / 1000.0f - 1), whitelist));
	}
}

void Robot::shootBigB() {

	// This is probably not the best option to whitelist all players
	std::vector<const Entity*> whitelist;
	for (Player *p : Game::sPlayers) {
		whitelist.push_back(p);
	}

	if (lastShotBigB > shootDelayBigB) {
		lastShotBigB = 0;
		Game::sBullets.push_back(new EnergyBullet(this->Get2DPos() + Util::RotationMat2(subEntities["body"]->GetAngle()) * bulletSpawn, subEntities["body"]->GetAngle() + accuracy * (rand() % 2000 / 1000.0f - 1), whitelist));
	}
}

void Robot::SetBodyAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());

	subEntities["body"]->SetRAngle(glm::mod<GLfloat>(subEntities["body"]->GetRAngle(), 2 * glm::pi<GLfloat>()));
	GLfloat dA = CalcMovAngle(subEntities["body"]->GetRAngle() + angle, bodyDir);
	if (abs(dA) > 0) {
		if (bodyTurnSpeed * dt > abs(dA)) {
			subEntities["body"]->SetRAngle(subEntities["body"]->GetRAngle() + dA);
			subEntities["shield"]->SetRAngle(subEntities["body"]->GetRAngle());
		}
		else {
			subEntities["body"]->SetRAngle(subEntities["body"]->GetRAngle() + dA / abs(dA) * bodyTurnSpeed * dt);
			subEntities["shield"]->SetRAngle(subEntities["body"]->GetRAngle());
		}
	}
}

void Robot::SetTrackAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());
	GLfloat dA = CalcMovAngle(angle, vel);

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

void Robot::SwitchToArtil()
{
	artilMode = GL_TRUE;
	dynFricCoeff = -50;
	accuracy = 0.01;
	shootDelay = 0.01;
}

void Robot::SwitchToNormal()
{
	artilMode = GL_FALSE;
	dynFricCoeff = -3;
	shootDelay = 0.1;
}