#pragma once

#include "Entity.h"

// Inital fricion still missing (High amount of Friction that only affects bodies at very low speeds)

class DynE : public Entity // Dynamic entity
{
public:
	DynE(std::string texture);
	~DynE();

	void addForce(glm::vec2 f); // the function that gets called if an external force acts upon the entity
	void updateE(float dt); // call this function every update

protected:
	GLfloat mass; // mass in kg
	GLfloat dynFricCoeff; // Coefficent of friction needs to be negative to counteract causing forces
	GLfloat statFricCoeff;
	GLfloat airFricCoeff;

	glm::vec2 force; // force in N
	glm::vec2 vel; // velocity in m/s
	glm::vec2 fricRes(); // calculates (linear) motion opposing force. Is needed to limit max speed;

};
