#pragma once

#include "Entity.h"

enum State {
	DYN_FRIC,
	NO_DYN_FRIC
};

class DynE : public Entity // Dynamic entity
{
public:
	DynE(glm::vec2 position);
	DynE(glm::vec2 position, GLfloat angle);
	DynE(glm::vec2 position, GLfloat angle, glm::vec2 vel);
	DynE(glm::vec2 position, GLfloat angle, GLfloat vel);

	virtual ~DynE();

	void AddForce(glm::vec2 f); // the function that gets called if an external force acts upon the entity
	void AddForce(glm::vec3 f);

	virtual GLboolean UpdateE(GLfloat dt); // call this function every update. returns true if moved

	// Should be private but its debugging	
	glm::vec2 Get2DVel() const;
	glm::vec3 GetVel() const;
	GLfloat GetAbsVel() const;
	GLfloat GetMass() const;

	void ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	void ColWithDyn(const DynE* dE, GLfloat penDepth, glm::vec2 colAxis);

protected:

	virtual void UpdatePos(GLfloat dt);

	State state;
	glm::vec3 vel; // velocity in m/s
	GLfloat airFricCoeff;
	GLboolean collision;
	GLfloat mass; // mass in kg
	GLfloat dynFricCoeff; // Coefficent of friction needs to be negative to counteract causing forces
	GLfloat statFricCoeff;
	glm::vec3 force; // force in N

	// Position and velocity after all collisions are done
	glm::vec2 colVel;

	// Utility functions
	glm::vec3 FricRes(); // calculates friction
	glm::vec3 AirRes(); // calculates air resistance
};