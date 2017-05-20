#pragma once

#include "Util.h"
#include "LivingE.h"

class Bullet : public DynE
{
public:
	Bullet(glm::vec2 position, GLfloat angle, GLfloat vel, std::vector<const Entity*> whitelist);
	virtual ~Bullet();

	GLboolean UpdateE(GLfloat dt);
	void ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithLivingE(LivingE* lE);
	void ColWithSubE(class SubE* sE, GLfloat penDepth, glm::vec2 colAxis);

protected:
	std::vector<const Entity*> whitelist; // Stores all entities that don't get damaged;
	GLfloat damage;

	// Utility functions
	GLboolean checkWL(const Entity* cLE) const;
};

