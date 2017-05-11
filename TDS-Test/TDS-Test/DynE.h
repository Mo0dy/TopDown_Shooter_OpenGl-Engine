#pragma once

#include "Entity.h"

enum State {
	STOPPING,
	MOVING
};

class DynE : public Entity // Dynamic entity
{
public:
	DynE(glm::vec2 position);
	DynE(glm::vec2 position, glm::vec2 size);
	DynE(glm::vec2 position, glm::vec2 size, GLfloat angle);
	DynE(glm::vec2 position, glm::vec2 size, std::string texture);
	DynE(glm::vec2 position, glm::vec2 size, GLfloat angle, std::string texture);
	virtual ~DynE();

	void addForce(glm::vec2 f); // the function that gets called if an external force acts upon the entity

	virtual GLboolean updateE(GLfloat dt); // call this function every update. returns true if moved
	virtual void ColWithDyn(DynE* cE, GLfloat colDepth, glm::vec2 minColAxis);
	virtual void ColWithStat(Entity* cE, GLfloat colDepth, glm::vec2 minColAxis);

	// Should be private but its debugging

	
	glm::vec2 getVel();
	GLfloat getAbsVel();
	GLfloat getMass();

protected:
	State state;
	glm::vec2 vel; // velocity in m/s
	GLfloat airFricCoeff;
	GLboolean collision;
	GLfloat mass; // mass in kg
	GLfloat dynFricCoeff; // Coefficent of friction needs to be negative to counteract causing forces
	GLfloat statFricCoeff;
	glm::vec2 force; // force in N

	// Position and velocity after all collisions are done
	glm::vec2 colVel;

	// Utility functions
	glm::vec2 fricRes(); // calculates friction
	glm::vec2 airRes(); // calculates air resistance
};