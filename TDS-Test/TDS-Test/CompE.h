#pragma once

#include "DynE.h"
#include "SubE.h"

class CompE: public DynE
{
public:
	CompE(glm::vec2 position);
	virtual ~CompE();

	std::map<std::string, SubE*> subEntities;
	std::vector<std::string> renderList;

	virtual void updateSupE();
	virtual void combineHitboxes();
	void updateAni();

	// Getters and setter
	void setColor(glm::vec3 color);
	void setSubESize(glm::vec2 size, std::string name);
	void setSubESize(GLfloat width, std::string name);
	std::map<std::string, SubE*> getSubE();
};

