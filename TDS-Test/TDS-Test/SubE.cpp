#include "SubE.h"

SubE::SubE(CompE* masterE) : masterE(masterE), rPos(glm::vec2(0)), rAngle(0)
{
	this->UpdateE(0);
}

SubE::SubE(CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec2 size) : masterE(masterE), rPos(rPos)
{
	this->size = size;
	Etex myEtex = *etex;
	myEtex.SetTexSize(size);
	this->tex = myEtex.GetTex();
	this->hitboxes = myEtex.GetHitObjs();
}

SubE::SubE(CompE* masterE, glm::vec2 rPos) : masterE(masterE), rPos(rPos), rAngle(0)
{
	this->UpdateE(0);
}

SubE::SubE(CompE* masterE, glm::vec2 rPos, GLfloat rAngle) : masterE(masterE), rPos(rPos), rAngle(rAngle)
{

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
