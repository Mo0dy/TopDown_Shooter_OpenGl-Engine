#include "SubE.h"

SubE::SubE(CompE* masterE) : masterE(masterE), rPos(glm::vec3(0)), rAngle(0)
{
	//this->UpdateE(0);
}
SubE::SubE(CompE* masterE, glm::vec2 rPos, const Etex* etex, GLfloat width, GLfloat height) : Entity(etex, width, height), masterE(masterE), rPos(glm::vec3(rPos, 0))
{
	//this->UpdateE(0);
}
SubE::SubE(CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec3 size) : Entity(etex, size), masterE(masterE), rPos(glm::vec3(rPos, 0))
{
	//this->UpdateE(0);
}
SubE::SubE(CompE* masterE, glm::vec2 rPos) : masterE(masterE), rPos(rPos, 0), rAngle(0)
{
	//this->UpdateE(0);
}
SubE::SubE(CompE* masterE, glm::vec2 rPos, GLfloat rAngle) : masterE(masterE), rPos(glm::vec3(rPos, 0)), rAngle(rAngle)
{
	//this->UpdateE(0);
}
SubE::~SubE() {}

GLboolean SubE::UpdateE(GLfloat dt)
{
	this->UpdatePos();
	return GL_TRUE;
}

glm::vec2 SubE::Get2DRPos() const { return glm::vec2(this->rPos.x, this->rPos.y); }
glm::vec3 SubE::GetRPos() const { return this->rPos; }
GLfloat SubE::GetRAngle() const
{
	return this->rAngle;
}
void SubE::Set2DRPos(glm::vec2 rPos)
{
	this->rPos.x = rPos.x;
	this->rPos.y = rPos.y;
}
void SubE::SetRPos(glm::vec3 rPos) { this->rPos = rPos; }
void SubE::SetRAngle(GLfloat rAngle) { this->rAngle = rAngle; }

void SubE::UpdatePos()
{
	this->angle = masterE->GetAngle() + rAngle;
	this->pos = glm::vec3(Util::RotationMat2(masterE->GetAngle()) * this->Get2DRPos() + masterE->Get2DPos(), masterE->GetZPos() + this->rPos.z);
}