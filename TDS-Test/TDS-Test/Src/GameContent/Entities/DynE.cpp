#include "DynE.h"

#include <iostream>
#include "..\..\OpenGl\Renderer.h"

DynE::DynE(glm::vec2 position) : Entity(position, 0.0f), mass(1), force(0), vel(0), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE) {}
DynE::DynE(glm::vec2 position, GLfloat angle) : Entity(position, angle), mass(1), force(0), vel(0), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE) {}
DynE::DynE(glm::vec2 position, GLfloat angle, glm::vec2 vel) : Entity(position, angle), mass(1), force(0), vel(glm::vec3(vel, 0)), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE) {}
DynE::DynE(glm::vec2 position, GLfloat angle, GLfloat vel) : Entity(position, angle), mass(1), force(0), vel(glm::vec3(-glm::sin(angle) * vel, glm::cos(angle) * vel, 0)), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE) {}

DynE::~DynE() {}

GLboolean DynE::UpdateE(GLfloat dt) {
	PreUpdate();
	UpdatePos(dt);
	return glm::length(vel) > 0;
}

void DynE::UpdatePos(GLfloat dt) {
	if (pos.z <= 1) { pos.z = 1; } // Heigt = 1 is the ground you cant go under that.
	else { state = NO_DYN_FRIC; } // No connection to the ground = no friction.
	// Updates velocity to after collision velocity
	if (collision) {
#ifdef DEBUG_FORCES
		Renderer::drawLineBuffer.push_back(myVertex(glm::vec2(pos.x, pos.y), glm::vec3(1.0f, 1.0f, 0.0f)));
		Renderer::drawLineBuffer.push_back(myVertex((glm::vec2(pos.x, pos.y) + (colVel - glm::vec2(vel.x, vel.z)) * mass / dt * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES
		vel = glm::vec3(colVel, vel.z);
		collision = GL_FALSE;
	}

	switch (state) {
	case NO_DYN_FRIC: AddForce(AirRes());
		break;
	case DYN_FRIC: AddForce(FricRes() + AirRes());
	}

	glm::vec3 dV = dt * force / mass;

	if (vel.x * (vel.x + dV.x) < 0 && vel.y * (vel.y + dV.y) < 0) {
		vel = glm::vec3(0);
	}
	else {
		vel += dV;
	}

	pos += dt * vel;

#ifdef DEBUG_FORCES
	Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
	Renderer::drawLineBuffer.push_back(myVertex((pos + force * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES

	force = glm::vec3(0);
}

// Utitlity functions
glm::vec3 DynE::FricRes() {
	if (glm::length(vel) > 0) {
		return glm::normalize(vel) * dynFricCoeff * mass * Util::GRAV_ACC; // Dynamic Friction
	}
	else {
		if (glm::length(force) > 0) {
			return glm::normalize(force) * statFricCoeff * mass * Util::GRAV_ACC; // Static Friction
		}
		else {
			return glm::vec3(0);
		}
	}
}

glm::vec3 DynE::AirRes() {
	if (glm::length(vel) > 0) {
		return glm::normalize(vel) * glm::pow(glm::length(vel), 2) * airFricCoeff;
	}
	return glm::vec3(0);
}

void DynE::ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis) {
	collision = GL_TRUE;
	glm::vec2 n = glm::normalize(colAxis);
	pos += glm::vec3(-n * penDepth, 0);
	colVel = vel;
}

void DynE::ColWithDyn(const class DynE* dE, GLfloat penDepth, glm::vec2 colAxis) {
	collision = GL_TRUE;
	glm::vec2 n = glm::normalize(colAxis);

	GLfloat v1p = glm::dot(glm::vec2(vel.x, vel.y), n);
	GLfloat v2p = glm::dot(dE->Get2DVel(), n);

	colVel = vel;

	//if ((v1p > 0 && v2p < v1p) || (v1p < 0 && v2p > v1p)) {
	colVel = vel - glm::vec3(dE->mass * Util::COEFFFICIENT_OF_RESTITUTION * (v1p - v2p) / (mass + dE->mass) * n, 0.0f); // <-- partly inelastiv collision
	//}
	pos += glm::vec3(-n * 0.5f * penDepth, 0);
}

// Getters and Setters
void DynE::AddForce(glm::vec2 f) { force += glm::vec3(f, 0); }
void DynE::AddForce(glm::vec3 f) { force += f; }
glm::vec2 DynE::Get2DVel() const { return glm::vec2(vel.x, vel.y);  }
glm::vec3 DynE::GetVel() const { return vel;  }
GLfloat DynE::GetAbsVel() const { return glm::length(vel);  }
GLfloat DynE::GetMass() const { return mass; }