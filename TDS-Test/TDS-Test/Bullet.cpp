#include "Bullet.h"

Bullet::Bullet(GLfloat vel, std::vector<Entity*> damageWhitelist) : DynE(glm::vec2(0), 0, vel)
{
	this->damageWhitelist.reserve(damageWhitelist.size());
	for (Entity *e : damageWhitelist) {
		this->damageWhitelist.push_back(e);
	}
	this->state = NO_DYN_FRIC;
}

Bullet::Bullet(glm::vec2 position, GLfloat angle, GLfloat vel, std::vector<const Entity*> damageWhitelist) : DynE(position, angle, vel), damageWhitelist(damageWhitelist)
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
	if (!checkColWL(e)) {
		this->collision = GL_TRUE;
	}
}

void Bullet::ColWithLivingE(LivingE* lE)
{
	if(!checkColWL(lE)) {
		collision = GL_TRUE;
		if (!checkDmgWL(lE)) {
			lE->GetAttacked(this->damage);
			this->damageWhitelist.push_back(lE);
		}
	}
}

void Bullet::ColWithSubE(class SubE* sE, GLfloat penDepth, glm::vec2 colAxis)
{
	collision = GL_TRUE;
	if (!checkDmgWL(sE->masterE)) {
		sE->GetAttacked(this->damage);
		this->damageWhitelist.push_back(sE->masterE);
	}
}

GLboolean Bullet::checkDmgWL(const Entity* cLE) const 
{
	for (const Entity *lE : damageWhitelist) {
		if (lE == cLE) {
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}

GLboolean Bullet::checkColWL(const Entity* cLE) const {
	for (const Entity *lE : collisionWhitelist) {
		if (lE == cLE) {
			return GL_TRUE;
		}
	}
	return GL_FALSE;
}