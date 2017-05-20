#include "Bullet.h"

Bullet::Bullet(glm::vec2 position, GLfloat angle, GLfloat vel, std::vector<const Entity*> whitelist) : DynE(position, angle, vel), whitelist(whitelist)
{
	this->state = NO_DYN_FRIC;
}

Bullet::~Bullet()
{
}

GLboolean Bullet::UpdateE(GLfloat dt) {
	if (collision) {
		this->erase = GL_TRUE;
		return GL_FALSE;
	}
	else {       
		this->pos += this->vel * dt;
		return GL_TRUE;
	}
}

void Bullet::ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis)
{
	this->collision = GL_TRUE;
}

void Bullet::ColWithLivingE(LivingE* lE)
{
	collision = GL_TRUE;
	if (!checkWL(lE)) {
		lE->GetAttacked(this->damage);
		this->whitelist.push_back(lE);
	}
}

void Bullet::ColWithSubE(class SubE* sE, GLfloat penDepth, glm::vec2 colAxis)
{
	collision = GL_TRUE;
	if (!checkWL(sE->masterE)) {
		sE->GetAttacked(this->damage);
		this->whitelist.push_back(sE->masterE);
	}
}

GLboolean Bullet::checkWL(const Entity* cLE) const 
{
	for (const Entity *lE : whitelist) {
		if (lE == cLE) {
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}