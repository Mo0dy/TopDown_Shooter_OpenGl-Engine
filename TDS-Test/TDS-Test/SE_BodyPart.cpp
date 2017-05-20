#include "SE_BodyPart.h"

SE_BodyPart::SE_BodyPart(CompE* masterE) : SubE(masterE)
{
}

SE_BodyPart::SE_BodyPart(CompE* masterE, glm::vec2 rPos, const Etex* etex, GLfloat width, GLfloat height) : SubE(masterE, rPos, etex, width, height)
{
}

SE_BodyPart::SE_BodyPart(CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec3 size) : SubE(masterE, rPos, etex, size)
{
}

SE_BodyPart::~SE_BodyPart()
{
}

void SE_BodyPart::GetAttacked(GLfloat damage)
{
	this->masterE->GetAttacked(damage);
}