#include "SE_Gun.h"

#include "Game.h"

SE_Gun::SE_Gun(class CompE* masterE, glm::vec2 rPos, Etex* etex, GLfloat width, GLfloat height, Bullet bullet, GLfloat shootDelay, GLfloat recoil, GLfloat accuracy, const std::vector<Enemy*> &damageWhitelist) : SubE(masterE, rPos, etex, width, height), bullet(bullet), shootDelay(shootDelay), recoil(recoil), lastShot(shootDelay), accuracy(accuracy)
{
	// adding Entities to the damage Whitelist
	this->damageWhitelist.reserve(damageWhitelist.size());
	for (Enemy *e : damageWhitelist) {
		this->damageWhitelist.push_back(e);
	}
	// avoids collision with itself
	this->colWhitelist.push_back(this);
}

SE_Gun::~SE_Gun() {}

void SE_Gun::Shoot() {
	if (lastShot >= shootDelay) {
		lastShot = 0;

		// This is probably not the best option to whitelist all players
		std::vector<const Entity*> colWhitelist;
		Game::sBullets.push_back(new EnergyBullet(this->Get2DPos(), this->GetAngle() + accuracy * (rand() % 2000 / 1000.0f - 1), damageWhitelist, colWhitelist));
	}
}

void SE_Gun::Reload() {
	bullets = magSize;
}
