#pragma once

#include "DynE.h"
#include "SubE.h"

class CompE: public DynE
{
public:
	CompE(glm::vec2 position);
	virtual ~CompE();

	std::map<std::string, SubE*> subEntities;
	std::vector<std::string> renderOrder;

	GLboolean UpdateE(GLfloat dt);
	virtual void UpdateSubE(GLfloat dt);
	void UpdateAni();

	// Getters and setter
	void SetColor(glm::vec3 color);
	void SetSubESize(glm::vec2 size, std::string name);
	void SetSubESize(GLfloat width, std::string name);
};