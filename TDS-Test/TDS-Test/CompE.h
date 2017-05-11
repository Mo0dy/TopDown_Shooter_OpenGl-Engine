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

	// Getters and setter
	void setColor(glm::vec3 color);
	std::map<std::string, SubE*> getSubE();
};

