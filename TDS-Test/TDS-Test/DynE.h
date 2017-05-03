#pragma once

#include "Entity.h"

// Inital fricion still missing (High amount of Friction that only affects bodies at very low speeds)

enum MovementState {
	RUNNING,
	SPRINTING,
	STOPPING
};

class DynE : public Entity // Dynamic entity
{
public:
	DynE(std::string texture);
	~DynE();
	void addForce(glm::vec2 f); // the function that gets called if an external force acts upon the entity
	void doStep(float dt); // call this function every update
	GLfloat getSelfPropForce(); // the enteties own force
	MovementState movState;
protected:
	GLfloat mass; // mass in kg
	GLfloat selfPropForce; // The force with wich the object propells it selfe
	GLfloat fricCoeff; // Coefficent of friction needs to be negative to counteract causing forces

	glm::vec2 force; // force in N
	glm::vec2 vel; // velocity in m/s
	glm::vec2 fricRes(); // calculates (linear) motion opposing force. Is needed to limit max speed;

};

