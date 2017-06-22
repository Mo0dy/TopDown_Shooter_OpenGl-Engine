#include "P_Ranger.h"
#include "..\..\..\..\Game.h"

void P_Ranger::LoadRanger()
{
	ResourceManager::LoadEtex("Textures", "Square", ".png", GL_TRUE, "Body", HBOX_AUTOFIT);
	ResourceManager::LoadEtex("Textures", "Rect", ".png", GL_TRUE, "Legs", HBOX_AUTOFIT);
	ResourceManager::LoadEtex("Textures", "Square", ".png", GL_TRUE, "Gun", HBOX_AUTOFIT);
}

P_Ranger::P_Ranger(glm::vec2 pos) : Player(pos)
{
	// Settings
	inherentForce = 2500;
	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;
	maxHealth = 2000;
	health = maxHealth;

	death = GL_FALSE;

	subEntities["body"] = new SE_BodyPart(this, glm::vec2(0), &ResourceManager::GetEtex("Body"), 2, 1);
	subEntities["legs"] = new SE_BodyPart(this, glm::vec2(0), &ResourceManager::GetEtex("Legs"), glm::vec3(1.0f, 0.3f, 1));

	subEntities["body"]->SetTurnSpeed(10);
	subEntities["legs"]->SetTurnSpeed(5);

	state = DYN_FRIC;
	SetColor(glm::vec3(1.0f));

	subEntities["gun"] = new SE_Gun(this, glm::vec2(0.5f, 0), &ResourceManager::GetEtex("Gun"), glm::vec3(0.2f, 4.0f, 0.2f), 0.1f, 1, 0.1f);

	renderOrder.push_back("legs");
	renderOrder.push_back("body");
	renderOrder.push_back("gun");

}

P_Ranger::~P_Ranger()
{
}

GLboolean P_Ranger::PreUpdate()
{
	movDir = glm::vec2(0);
	bodyDir = glm::vec2(0);
	if (this->health <= 0) {
		death = GL_TRUE;
	}
	if (this->death) {
		return GL_TRUE;
	}
	return GL_FALSE;
}

GLboolean P_Ranger::UpdateE(GLfloat dt)
{
	if (PreUpdate()) {
		return GL_FALSE;
	}
	SetColor(glm::vec3(1 - health / maxHealth, color.y * health / maxHealth, color.z * health / maxHealth));
	movState = NORMAL;
	state = DYN_FRIC;

	// Updating body direction
	if (abs(gPad.sThumbRX) > Util::CONTROLLER_DEADZONE || abs(gPad.sThumbRY) > Util::CONTROLLER_DEADZONE) {
		bodyDir += glm::vec2(gPad.sThumbRX, 0);
		bodyDir += glm::vec2(0, gPad.sThumbRY);
	}
	// Normal Mode
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
		break;
	case NORMAL:
		this->AddForce(movDir * inherentForce);
		break;
	case SPRINTING:
		this->AddForce(movDir * inherentForce * (1.0f + 3.0f / Util::CONTROLLER_TRIGGER_MAX * (GLfloat)gPad.bLeftTrigger));
		break;
	}
	if (gPad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		std::vector<const Entity*> tDmgWhitelist;
		for (Player *p : Game::sPlayers) {
			tDmgWhitelist.push_back(p);
			for (auto &x : p->subEntities) {
				tDmgWhitelist.push_back(x.second);
			}
		}
		dynamic_cast<SE_Gun*>(subEntities["gun"])->Shoot(tDmgWhitelist);
	}
	subEntities["legs"]->LookAt(movDir, dt);


	subEntities["body"]->LookAt(bodyDir, dt);
	subEntities["gun"]->SetRAngle(subEntities["body"]->GetRAngle());
	UpdatePos(dt);
	UpdateSubE(dt);
	return glm::length(vel) > 0;
}
