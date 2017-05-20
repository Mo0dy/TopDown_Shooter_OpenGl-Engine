#include "SubE.h"

SubE::SubE(CompE* masterE) : masterE(masterE), rPos(glm::vec2(0)), rAngle(0)
{
	//this->UpdateE(0);
}

SubE::SubE(CompE* masterE, glm::vec2 rPos, const Etex* etex, GLfloat width) : Entity(etex, width), masterE(masterE), rPos(rPos)
{
	//this->UpdateE(0);
}

SubE::SubE(CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec2 size) : Entity(etex, size), masterE(masterE), rPos(rPos)
{
	//this->UpdateE(0);
}

SubE::SubE(CompE* masterE, glm::vec2 rPos) : masterE(masterE), rPos(rPos), rAngle(0)
{
	//this->UpdateE(0);
}

SubE::SubE(CompE* masterE, glm::vec2 rPos, GLfloat rAngle) : masterE(masterE), rPos(rPos), rAngle(rAngle)
{
	//this->UpdateE(0);
}

SubE::~SubE()
{
}

GLboolean SubE::UpdateE(GLfloat dt)
{
	pos = masterE->GetPos() + rPos;
	angle = masterE->GetAngle() + rAngle;
	return GL_TRUE;
}

glm::vec2 SubE::GetRPos() const
{
	return this->rPos;
}
GLfloat SubE::GetRAngle() const
{
	return this->rAngle;
}
void SubE::SetRPos(glm::vec2 rPos)
{
	this->rPos = rPos;
}
void SubE::SetRAngle(GLfloat rAngle) 
{
	this->rAngle = rAngle;
}

void SubE::UpdatePos()
{
	this->angle = masterE->GetAngle() + rAngle;
	this->pos = Util::RotationMat2(masterE->GetAngle()) * rPos + masterE->GetPos();
}