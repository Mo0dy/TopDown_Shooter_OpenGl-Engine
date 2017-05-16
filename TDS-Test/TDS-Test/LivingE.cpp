#include "LivingE.h"

LivingE::LivingE(glm::vec2 position): CompE(position)
{
}

LivingE::~LivingE()
{
}

GLboolean LivingE::UpdateE(GLfloat dt)
{
	if (this->health <= 0) {
		death = GL_TRUE;
		erase = GL_TRUE;
	}
	return GL_FALSE;
}

void LivingE::GetAttacked(GLfloat damage) {
	this->health -= damage;
}

// Getters and setters
GLboolean LivingE::GetDeath() const { return death;  }
GLfloat LivingE::GetInherentF() const { return inherentF; }