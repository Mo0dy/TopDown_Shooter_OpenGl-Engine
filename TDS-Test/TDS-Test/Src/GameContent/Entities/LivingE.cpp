#include "LivingE.h"

LivingE::LivingE(glm::vec2 position): CompE(position)
{
}

LivingE::~LivingE()
{
}

GLboolean LivingE::PreUpdate() {
	if (this->health <= 0) {
		death = GL_TRUE;
	}
	if (this->death) {
		erase = GL_TRUE;
		return GL_TRUE;
	}
	return GL_FALSE;
}

GLboolean LivingE::UpdateE(GLfloat dt)
{
	if (PreUpdate()) { return GL_FALSE; }
	UpdatePos(dt);
	UpdateSubE(dt);
	return glm::length(vel) > 0;
}

void LivingE::GetAttacked(GLfloat damage) {
	this->health -= damage;
}

// Getters and setters
GLboolean LivingE::GetDeath() const { return death;  }
GLfloat LivingE::GetInherentF() const { return inherentF; }