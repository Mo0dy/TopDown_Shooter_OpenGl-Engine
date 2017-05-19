#include "Shield.h"

void Shield::LoadShield()
{
	ResourceManager::LoadEtex("Textures", "Shield", ".png", GL_TRUE, "Shield", HBOX_AUTOFIT);
}

Shield::Shield(class CompE* masterE) : SubE(masterE),maxShieldHP(2000), shieldHP(maxShieldHP), active(GL_FALSE), shieldSize(glm::vec2(5))
{
	this->size = glm::vec2(0);
	tex = ResourceManager::GetEtex("Shield").GetTex();
	shieldHObjs = ResourceManager::GetEtex("Shield").GetHitObjs(shieldSize);
	rechargePerSec = 300;
}

Shield::~Shield()
{
}

GLboolean Shield::UpdateE(GLfloat dt)
{
	if (shieldHP < 0) {
		shieldHP = 0;
		StopShield();
	}
	if (!active) {
		shieldHP += rechargePerSec * dt;
		if (shieldHP > maxShieldHP) {
			shieldHP = maxShieldHP;
		}
	}
	else {
		angle += 5 * dt;
		size = shieldSize + shieldSize * 0.1f * (float) glm::sin(glfwGetTime());
	}
	pos = masterE->GetPos() + rPos;
	return GL_FALSE;
}

void Shield::StartShield()
{
	if (shieldHP > 200) {
		this->size = shieldSize;
		hitObjs = shieldHObjs;
		active = GL_TRUE;
	}
}

void Shield::StopShield()
{
	this->size = glm::vec2(0);
	hitObjs.clear();
	active = GL_FALSE;
}

void Shield::ToggleShield()
{
	if (active) {
		StopShield();
	}
	else {
		StartShield();
	}
}

void Shield::GetAttacked(GLfloat damage)
{
	this->shieldHP -= damage;
}