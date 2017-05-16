#include "LivingE.h"

LivingE::LivingE(glm::vec2 position): CompE(position)
{
}

LivingE::~LivingE()
{
}

void LivingE::AddDamage(GLfloat damage) {
	health -= damage;
}

// Getters and setters
GLboolean LivingE::GetDeath() { return death;  }