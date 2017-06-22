#pragma once

#include "..\..\..\Tools\Util.h"
#include "..\LivingE.h"

class Bullet : public DynE
{
public:
	Bullet(GLfloat vel, std::vector<const Entity*> damageWhitelist);
	Bullet(glm::vec2 position, GLfloat angle, GLfloat vel, std::vector<const Entity*> damageWhitelist);
	virtual ~Bullet();

	GLboolean UpdateE(GLfloat dt);
	void ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithLivingE(LivingE* lE);
	void ColWithSubE(class SubE* sE, GLfloat penDepth, glm::vec2 colAxis);

protected:
	std::vector<const Entity*> damageWhitelist; // Stores all entities that don't get damaged;
	std::vector<const Entity*> collisionWhitelist; // Stores all entities that cant be collidet with;
	GLfloat damage;

	// Utility functions
	GLboolean checkDmgWL(const Entity* cLE) const;
	GLboolean checkColWL(const Entity* cLE) const;
};

