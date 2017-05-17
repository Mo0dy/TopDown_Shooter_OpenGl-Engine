#pragma once

#include "SubE.h"

class SE_BodyPart : public SubE
{
public:
	SE_BodyPart(CompE* masterE);
	SE_BodyPart(CompE* masterE, glm::vec2 rPos, const Etex* etex, GLfloat width);
	SE_BodyPart(CompE* masterE, glm::vec2 rPos, const Etex* etex, glm::vec2 size);
	~SE_BodyPart();

	void GetAttacked(GLfloat damage);
};

