#include "DynE.h"

#include <iostream>
#include "Renderer.h"

DynE::DynE(glm::vec2 position) : Entity(position, 0.0f), mass(1), force(0), vel(0), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE)
{
}

DynE::DynE(glm::vec2 position, GLfloat angle) : Entity(position, angle), mass(1), force(0), vel(0), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE)
{
}

DynE::DynE(glm::vec2 position, GLfloat angle, glm::vec2 vel) : Entity(position, angle), mass(1), force(0), vel(vel), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE)
{
}

DynE::DynE(glm::vec2 position, GLfloat angle, GLfloat vel) : Entity(position, angle), mass(1), force(0), vel(glm::vec2(glm::cos(angle), glm::sin(angle)) * vel), dynFricCoeff(0), statFricCoeff(0), airFricCoeff(0), state(DYN_FRIC), collision(GL_FALSE)
{
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
	case NO_DYN_FRIC: AddForce(AirRes());
		break;
	case DYN_FRIC: AddForce(FricRes() + AirRes());
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
	else { // if goalVec.x == 0 you can either go exactly to the reight (goalAngle = 0) or to the left (goalAngle = PI)
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

void DynE::ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis) {
	collision = GL_TRUE;
	glm::vec2 n = glm::normalize(colAxis);
	pos += -n * penDepth;
	colVel = vel;
}

void DynE::ColWithDyn(const class DynE* dE, GLfloat penDepth, glm::vec2 colAxis) {
	collision = GL_TRUE;
	glm::vec2 n = glm::normalize(colAxis);

	GLfloat v1p = glm::dot(vel, n);
	GLfloat v2p = glm::dot(dE->GetVel(), n);

	colVel = vel;

	//if ((v1p > 0 && v2p < v1p) || (v1p < 0 && v2p > v1p)) {
	colVel = vel - dE->mass * Util::COEFFFICIENT_OF_RESTITUTION * (v1p - v2p) / (mass + dE->mass) * n; // <-- partly inelastiv collision
	//}
	pos += -n * 0.5f * penDepth;
}

// Getters and Setters
void DynE::AddForce(glm::vec2 f) { force += f; }
glm::vec2 DynE::GetVel() const { return vel;  }
GLfloat DynE::GetAbsVel() const { return glm::length(vel);  }
GLfloat DynE::GetMass() const { return mass; }

