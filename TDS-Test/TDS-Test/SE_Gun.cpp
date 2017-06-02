#include "SE_Gun.h"

SE_Gun::SE_Gun(class CompE* masterE, glm::vec2 rPos, Bullet bullet, GLfloat shootDelay, GLfloat recoil) : SubE(masterE, rPos), bullet(bullet), shootDelay(shootDelay), recoil(recoil), lastShot(shootDelay) {}

SE_Gun::~SE_Gun() {}

void SE_Gun::Shoot() {
	//if (lastShot >= shootDelay) {
	//	lastShot = 0;

	//	// This is probably not the best option to whitelist all players
	//	std::vector<const Entity*> damageWhitelist;
	//	std::vector<const Entity*> colWhitelist;
	//	colWhitelist.push_back(this);
	//	damageWhitelist.reserve(Game::sEnemies.size());
	//	for (Enemy *e : Game::sEnemies) {
	//		damageWhitelist.push_back(e);
	//	}
	//	Game::sBullets.push_back(new EnergyBullet(this->Get2DPos(), subEntities["gun"]->GetAngle() + accuracy * (rand() % 2000 / 1000.0f - 1), damageWhitelist, colWhitelist));
	//}
}

void SE_Gun::Reload() {
	bullets = magSize;
}
