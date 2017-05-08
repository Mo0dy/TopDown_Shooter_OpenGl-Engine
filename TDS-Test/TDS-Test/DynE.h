#pragma once

#include "Entity.h"

// Inital fricion still missing (High amount of Friction that only affects bodies at very low speeds)

enum State {
	STOPPING,
	MOVING
};

class DynE : public Entity // Dynamic entity
{
public:
	DynE(glm::vec2 position);
	virtual ~DynE();

	void addForce(glm::vec2 f); // the function that gets called if an external force acts upon the entity
	virtual GLboolean updateE(GLfloat dt); // call this function every update. returns true if moved
	virtual void Collision(Entity* cE, GLfloat dt);
	State state;

	// Should be private but its debugging
	glm::vec2 vel; // velocity in m/s
	GLfloat airFricCoeff;
	GLboolean collision;
protected:
	GLfloat mass; // mass in kg
	GLfloat dynFricCoeff; // Coefficent of friction needs to be negative to counteract causing forces
	GLfloat statFricCoeff;
	glm::vec2 force; // force in N

	// Position and velocity after all collisions are done
	glm::vec2 colVel; 
	glm::vec2 colPos;

	// Utility functions
	glm::vec2 fricRes(); // calculates friction
	glm::vec2 airRes(); // calculates air resistance
};