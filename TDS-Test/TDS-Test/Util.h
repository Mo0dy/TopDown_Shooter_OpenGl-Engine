#pragma once

#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>

#include <iostream>
#include <vector>
#include <string>

// Settings
#define CONTROLLER_SUPPORT
#define KEYBOARD_SUPPORT

//#define SECOND_PLAYER

//#define FULLSCREEN

// Debug
#define DEBUG
#ifdef DEBUG
	//#define DEBUG_FORCES
	#define LOG(x) std::cout << x << std::endl
#endif

class Util {
public:
	static GLfloat calcMovAngle(GLfloat currAngle, glm::vec2 goalVec) {
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

		LOG("A = " << currAngle << " | gA = " << goalAngle << " | dA = " << dA);

		return dA;
	}

};
// Physics
static const GLfloat GRAV_ACC = 9.81f;

// Sudo Physics
static const GLfloat COLLISION_ADD_CHANGE = 0.001;

// Rendering
static const GLfloat FORCE_SCALE = 0.001f;

static void printVec2(glm::vec2 v) {
	std::cout << "[" << v.x << ", " << v.y << "]" << std::endl;
}

// Other
static const GLfloat CONTROLLER_STICK_MAX = 32767;

// Utility funciton