#pragma once

#include "..\..\Tools\Util.h"

#include "..\Entities\Entity.h"
#include "..\..\OpenGl\ResourceManager.h"

class Level
{
public:
	Level();
	~Level();
	glm::vec2 size;
	Entity background;

	virtual void UpdateL(GLfloat dt);
	virtual void Reset();
};

