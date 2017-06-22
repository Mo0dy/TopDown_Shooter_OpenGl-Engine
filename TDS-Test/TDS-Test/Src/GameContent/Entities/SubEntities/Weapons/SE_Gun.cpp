#include "SE_Gun.h"
#include "..\..\..\..\Game.h"
#include "..\..\Bullets\EnergyBullet.h"

SE_Gun::SE_Gun(CompE* masterE, glm::vec2 rPos, Etex* etex, GLfloat width, GLfloat height, GLfloat shootDelay, GLfloat recoil, GLfloat accuracy) : SubE(masterE, rPos, etex, width, height), shootDelay(shootDelay), recoil(recoil), lastShot(shootDelay), accuracy(accuracy) {

	// Creating collision whitelist from all entities contained in the masterE
	colWhitelist.push_back(this);
	colWhitelist.push_back(masterE);
	for (const auto &x : masterE->subEntities) {
		colWhitelist.push_back(x.second);
	}
}

SE_Gun::SE_Gun(CompE* masterE, glm::vec2 rPos, Etex* etex, glm::vec3 size, GLfloat shootDelay, GLfloat recoil, GLfloat accuracy) : SubE(masterE, rPos, etex, size), shootDelay(shootDelay), recoil(recoil), lastShot(shootDelay), accuracy(accuracy) {

	// Creating collision whitelist from all entities contained in the masterE
	colWhitelist.push_back(this);
	colWhitelist.push_back(masterE);
	for (const auto &x : masterE->subEntities) {
		colWhitelist.push_back(x.second);
	}
}

SE_Gun::~SE_Gun() {}

GLboolean SE_Gun::UpdateE(GLfloat dt)
{
	PreUpdate();
	this->UpdatePos();
	lastShot += dt;
	return GL_TRUE;
}

void SE_Gun::Shoot(std::vector<const Entity*> dmgWhitelist) {
	if (lastShot >= shootDelay) {
		lastShot = 0;

		// This is probably not the best option to whitelist all players
		Game::sBullets.push_back(new EnergyBullet(this->Get2DPos(), this->GetAngle() + accuracy * (rand() % 2000 / 1000.0f - 1), dmgWhitelist, colWhitelist));
	}
}

void SE_Gun::Reload() {
	bullets = magSize;
}
