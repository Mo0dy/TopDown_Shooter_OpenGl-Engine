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
	~DynE();

	void addForce(glm::vec2 f); // the function that gets called if an external force acts upon the entity
	virtual GLboolean updateE(float dt); // call this function every update. returns true if moved
	virtual void Collision(Entity* cE, GLfloat dt);
	State state;
protected:
	GLfloat mass; // mass in kg
	GLfloat dynFricCoeff; // Coefficent of friction needs to be negative to counteract causing forces
	GLfloat statFricCoeff;
	GLfloat airFricCoeff;
	glm::vec2 force; // force in N
	glm::vec2 vel; // velocity in m/s

	// Utility functions
	glm::vec2 fricRes(); // calculates friction
	glm::vec2 airRes(); // calculates air resistance

};

