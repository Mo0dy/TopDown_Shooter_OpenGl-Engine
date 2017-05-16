#pragma once

#include "Util.h"
#include "LivingE.h"

class Bullet : public DynE
{
public:
	Bullet(glm::vec2 position, GLfloat angle, GLfloat vel, std::vector<const LivingE*> whitelist);
	virtual ~Bullet();

	GLboolean UpdateE(GLfloat dt);
	void ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithLivingE(LivingE* lE);

protected:
	std::vector<const LivingE*> whitelist; // Stores all entities that don't get damaged;
	GLfloat damage;
	GLfloat vel;

	// Utility functions
	GLboolean checkWL(const LivingE* cLE);
};

