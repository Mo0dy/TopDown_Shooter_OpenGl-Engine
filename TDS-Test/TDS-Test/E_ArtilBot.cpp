#include "E_ArtilBot.h"
#include "Game.h"

void E_ArtilBot::Load_E_ArtilBot() {
	ResourceManager::LoadEtex("Textures", "Square", ".png", GL_TRUE, "Square", HBOX_AUTOFIT);
	ResourceManager::LoadEtex("Textures", "Rect", ".png", GL_TRUE, "Rect", HBOX_AUTOFIT);
}

E_ArtilBot::E_ArtilBot(glm::vec2 position) : Enemy(position) 
{
	maxHealth = 150;
	health = maxHealth;
	mass = 100;
	movForce = 200;
	state = DYN_FRIC;
	airFricCoeff = -1;
	dynFricCoeff = -10;

	damage = 400;
	attackSpeed = 0.2;
	turnSpeed = 10;
	gunTurnSpeed = 10;

	accuracy = 0.1;

	size = glm::vec3(1.0f);

	tex = ResourceManager::GetEtex("Square").GetTex();
	hitComb = ResourceManager::GetEtex("Square").GetHitComb(this->size);

	subEntities["gun"] = new SE_BodyPart(this, glm::vec2(0), &ResourceManager::GetEtex("Rect"), 0.2f, 0.5f);

	renderOrder.push_back("gun");
}

E_ArtilBot::~E_ArtilBot() {}

GLboolean E_ArtilBot::UpdateE(GLfloat dt)
{
	if (!death) {
		state = DYN_FRIC;
		lastAttack += dt;
		if (health <= 0) {
			death = GL_TRUE;
			return GL_FALSE;
		}

		// finding the closest Player
		Player* gPlayer = Game::sPlayers.front();
		GLfloat gPdist = glm::distance(gPlayer->Get2DPos(), this->Get2DPos());
		for (Player* p : Game::sPlayers) {
			if (glm::distance(p->Get2DPos(), this->Get2DPos()) < gPdist) {
				gPlayer = p;
				gPdist = glm::distance(gPlayer->Get2DPos(), this->Get2DPos());
			}
		}

		this->gunDir = gPlayer->Get2DPos() - this->Get2DPos();

		if (gPdist < 20) {
			this->AddForce(glm::normalize(gunDir) * -movForce);
			state = NO_DYN_FRIC;
		}
		else {
			this->AddForce(glm::normalize(gunDir) * movForce);
			state = NO_DYN_FRIC;
		}

		SetGunAngle(dt);
		UpdatePos(dt);
		UpdateSubE(dt);
		Shoot();
		return glm::length(vel) > 0;
	}
}

void E_ArtilBot::SetGunAngle(GLfloat dt) {
	angle = glm::mod<GLfloat>(angle, 2 * glm::pi<GLfloat>());

	subEntities["gun"]->SetRAngle(glm::mod<GLfloat>(subEntities["gun"]->GetRAngle(), 2 * glm::pi<GLfloat>()));
	GLfloat dA = CalcMovAngle(subEntities["gun"]->GetRAngle() + angle, gunDir);
	if (abs(dA) > 0) {
		if (gunTurnSpeed * dt > abs(dA)) {
			subEntities["gun"]->SetRAngle(subEntities["gun"]->GetRAngle() + dA);
		}
		else {
			subEntities["gun"]->SetRAngle(subEntities["gun"]->GetRAngle() + dA / abs(dA) * gunTurnSpeed * dt);
		}
	}
}

void E_ArtilBot::Shoot() {
	if (lastAttack > attackSpeed) {
		lastAttack = 0;

		// This is probably not the best option to whitelist all players
		std::vector<const Entity*> damageWhitelist;
		std::vector<const Entity*> colWhitelist;
		colWhitelist.push_back(this);
		damageWhitelist.reserve(Game::sEnemies.size());
		for (Enemy *e : Game::sEnemies) {
			damageWhitelist.push_back(e);
		}
		Game::sBullets.push_back(new EnergyBullet(this->Get2DPos(), subEntities["gun"]->GetAngle() + accuracy * (rand() % 2000 / 1000.0f - 1), damageWhitelist, colWhitelist));
	}
}