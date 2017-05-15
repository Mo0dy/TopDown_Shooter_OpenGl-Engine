#include "LivingE.h"

LivingE::LivingE(glm::vec2 position): CompE(position)
{
}

LivingE::~LivingE()
{
}

void LivingE::GetAttacked(GLfloat damage) {
	health -= damage;
}

// Getters and setters
GLboolean LivingE::getDeath() { return death;  }