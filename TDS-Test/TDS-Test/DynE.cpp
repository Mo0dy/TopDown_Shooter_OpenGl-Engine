#include "DynE.h"

#include <iostream>
#include "Renderer.h"

DynE::DynE(glm::vec2 position) : Entity(position), mass(10), force(0, 0), vel(0, 0), dynFricCoeff(-0.5f), statFricCoeff(-0.7f), airFricCoeff(-1.0f), state(STOPPING), collision(GL_FALSE)
{
}

DynE::DynE(glm::vec2 position, GLfloat angle) : DynE(position) {
	this->angle = angle;
}

DynE::~DynE()
{
}

GLboolean DynE::UpdateE(GLfloat dt) {
	UpdatePos(dt);
	return glm::length(vel) > 0;
}

void DynE::UpdatePos(GLfloat dt) {
	if (collision) {
#ifdef DEBUG_FORCES
		Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
		Renderer::drawLineBuffer.push_back(myVertex((pos + (colVel - vel) * mass / dt * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES
		vel = colVel;
		collision = GL_FALSE;
	}

	switch (state) {
	case MOVING: AddForce(AirRes());
		break;
	case STOPPING: AddForce(FricRes() + AirRes());
	}

	glm::vec2 dV = dt * force / mass;

	if (vel.x * (vel.x + dV.x) < 0 && vel.y * (vel.y + dV.y) < 0) {
		vel = glm::vec2(0, 0);
	}
	else {
		vel += dV;
	}

	pos += dt * vel;

#ifdef DEBUG_FORCES
	Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
	Renderer::drawLineBuffer.push_back(myVertex((pos + force * FORCE_SCALE), glm::vec3(1.0f, 1.0f, 0.0f)));
#endif // DEBUG_FORCES

	force = glm::vec2(0, 0);
}

// Utitlity functions
glm::vec2 DynE::FricRes() {
	if (glm::length(vel) > 0) {
		return glm::normalize(vel) * dynFricCoeff * mass * Util::GRAV_ACC; // Dynamic Friction
	}
	else {
		if (glm::length(force) > 0) {
			return glm::normalize(force) * statFricCoeff * mass * Util::GRAV_ACC; // Static Friction
		}
		else {
			return glm::vec2(0, 0);
		}
	}
}

glm::vec2 DynE::AirRes() {
	if (glm::length(vel) > 0) {
		return glm::normalize(vel) * glm::pow(glm::length(vel), 2) * airFricCoeff;
	}
	return glm::vec2(0, 0);
}

GLfloat DynE::CalcMovAngle(GLfloat currAngle, glm::vec2 goalVec) {
	goalVec = glm::normalize(goalVec);
	GLfloat goalAngle;
	if (goalVec.x != 0) {
		goalAngle = glm::mod<float>(2 * glm::pi<GLfloat>() - glm::acos(goalVec.y) * goalVec.x / abs(goalVec.x), 2 * glm::pi<GLfloat>());
	}
	else {
		if (goalVec.y > 0) {
			goalAngle = 0;
		}
		else {
			goalAngle = glm::pi<GLfloat>();
		}
	}

	GLfloat dA = goalAngle - currAngle;

	if (abs(dA) > glm::pi<GLfloat>()) {
		if (dA > 0) {
			dA -= 2 * glm::pi<GLfloat>();
		}
		else {
			dA += 2 * glm::pi<GLfloat>();
		}
	}
	return dA;
}

// Getters and Setters
void DynE::AddForce(glm::vec2 f) { force += f; }
glm::vec2 DynE::GetVel() { return vel;  }
GLfloat DynE::GetAbsVel() { return glm::length(vel);  }
GLfloat DynE::GetMass() { return mass; }