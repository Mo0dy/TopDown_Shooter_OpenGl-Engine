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
	PreUpdate();
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

void SubE::LookAt(glm::vec2 goalVec, GLfloat dt) {
	goalVec = glm::normalize(goalVec);
	GLfloat goalAngle;
	if (goalVec.x != 0) {
		goalAngle = glm::mod<float>(2 * glm::pi<GLfloat>() - glm::acos(goalVec.y) * goalVec.x / abs(goalVec.x), 2 * glm::pi<GLfloat>());
	}
	else { // if goalVec.x == 0 you can either go exactly to the reight (goalAngle = 0) or to the left (goalAngle = PI)
		if (goalVec.y > 0) {
			goalAngle = 0;
		}
		else { goalAngle = glm::pi<GLfloat>(); }
	}
	LookAt(goalAngle, dt);
}

void SubE::LookAt(GLfloat goalAngle, GLfloat dt) {
	rAngle = glm::mod<GLfloat>(rAngle + glm::pi<GLfloat>() * 2, glm::pi<GLfloat>() * 2);
	GLfloat dA = goalAngle - angle - masterE->GetAngle();
	if (dA > glm::pi<GLfloat>()) {
		dA -= 2 * glm::pi<GLfloat>();
	}
	else if (dA < -glm::pi<GLfloat>()) {
		dA += 2 * glm::pi<GLfloat>();
	}
	if (abs(dA) > 0) {
		if (turnSpeed * dt > abs(dA)) {
			this->rAngle += dA;
		}
		else {
			this->rAngle += dA / abs(dA) * turnSpeed * dt;
		}
	}
}

void SubE::UpdatePos()
{
	this->angle = masterE->GetAngle() + rAngle;
	this->pos = glm::vec3(Util::RotationMat2(masterE->GetAngle()) * this->Get2DRPos() + masterE->Get2DPos(), masterE->GetZPos() + this->rPos.z);
}