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
// #define KEYBOARD_SUPPORT

#define CAM_STANDARD_SIZE 10

//#define SECOND_PLAYER


// Debug
#define DEBUG
#ifdef DEBUG
	#define CAM_ZOOM_SPEED 0.2
	//#define DEBUG_FORCES
	#define LOG(x) std::cout << x << std::endl
#endif

// utility funciton
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
		return dA;
	}
	
	static glm::mat2 create2DrotMatrix(GLfloat angle) {
		glm::mat2 rotMat;
		rotMat[0][0] = cos(angle / 180 * glm::pi<GLfloat>());
		rotMat[1][0] = -sin(angle / 180 * glm::pi<GLfloat>());
		rotMat[0][1] = sin(angle / 180 * glm::pi<GLfloat>());
		rotMat[1][1] = cos(angle / 180 * glm::pi<GLfloat>());
		return rotMat;
	}

	static void printVec2(glm::vec2 v) {
		std::cout << "[" << v.x << ", " << v.y << "]" << std::endl;
	}
};

// Physics
static const GLfloat GRAV_ACC = 9.81f;

// Sudo Physics
static const GLfloat COLLISION_ADD_CHANGE = 0.01f;

// Rendering
#define FULLSCREEN
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
static const GLfloat FORCE_SCALE = 0.001f;

// Other
static const GLfloat CONTROLLER_STICK_MAX = 32767.0f;
static const GLfloat CONTROLLER_TRIGGER_MAX = 255.0f;

// Utility funciton

// Input
#define CONTROLLER_DEADZONE 6500