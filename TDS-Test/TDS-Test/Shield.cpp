#include "Shield.h"

void Shield::LoadShield()
{
	ResourceManager::LoadEtex("Textures", "ShieldCone", ".png", GL_TRUE, "Shield", HBOX_AUTOFIT);
}

Shield::Shield(class CompE* masterE) : SubE(masterE),maxShieldHP(2000), shieldHP(maxShieldHP), active(GL_FALSE), shieldSize(glm::vec3(ResourceManager::GetEtex("Shield").GetTexSize(3), masterE->GetZSize()))
{
	this->size = glm::vec3(0);
	this->rPos = glm::vec3(0, 1.0f, 0);
	tex = ResourceManager::GetEtex("Shield").GetTex();
	shieldHObjs = ResourceManager::GetEtex("Shield").GetHitComb(shieldSize);
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
		UpdatePos();
		size = shieldSize + shieldSize * 0.1f * (float) glm::sin(glfwGetTime());
	}
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
	this->size = glm::vec3(0);
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

void Shield::UpdatePos()
{
	this->angle = masterE->GetAngle() + rAngle;
	this->pos = glm::vec3(Util::RotationMat2(masterE->GetAngle() + this->rAngle) * this->Get2DRPos() + masterE->Get2DPos(), masterE->GetZPos() + this->rPos.z);
}