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

// Controles
static const GLboolean CONTROLLER_SUPPORT = GL_TRUE;
// #define KEYBOARD_SUPPORT

// Camera
static const GLfloat CAM_STANDARD_SIZE = 10;
static const GLfloat CAM_ZOOM_SPEED = 0.2;

#ifndef DEBUG
	static const GLfloat CAM_MAX_ZOOM = 25;
#else // !DEBUG
	static const GLfloat CAM_MAX_ZOOM = 60;
#endif // !DEBUG

// Game
static const GLuint PLAYER_AMOUNT = 2;
static const glm::vec3 PLAYER_COLOR[] = {
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f),
	glm::vec3(1.0f)
};

// Physics constants
static const GLfloat GRAV_ACC = 9.81f;
// Sudo Physics
static const GLfloat COLLISION_ADD_CHANGE = 0.001f;

// Rendering
static const GLboolean FULLSCREEN = GL_TRUE;
static const GLuint SCREEN_WIDTH = 1920;
static const GLuint SCREEN_HEIGHT = 1080;

// Other
static const GLfloat CONTROLLER_STICK_MAX = 32767.0f;
static const GLfloat CONTROLLER_TRIGGER_MAX = 255.0f;
static const GLfloat CONTROLLER_DEADZONE = 6500.0f;

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


// Debug
#define DEBUG
#ifdef DEBUG
#define LOG_FPS
//#define DEBUG_FORCES
#define LOG(x) std::cout << x << std::endl
#endif

#ifdef DEBUG_FORCES
#define FORCE_SCALE  0.001f
#endif // DEBUG_FORCES