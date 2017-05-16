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

	virtual GLboolean UpdateE(GLfloat dt); // call this function every update. returns true if moved

	// Should be private but its debugging	
	glm::vec2 GetVel() const;
	GLfloat GetAbsVel() const;
	GLfloat GetMass() const;

	void ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	void ColWithDyn(const DynE* dE, GLfloat penDepth, glm::vec2 colAxis);

protected:
	virtual void UpdatePos(GLfloat dt);
	virtual GLfloat CalcMovAngle(GLfloat currAngle, glm::vec2 goalVec);

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
	glm::vec2 FricRes(); // calculates friction
	glm::vec2 AirRes(); // calculates air resistance
};