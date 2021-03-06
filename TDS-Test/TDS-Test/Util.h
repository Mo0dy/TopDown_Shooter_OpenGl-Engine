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



// utility funciton
class Util {
public:
	// Settings

	// Controles
	static GLboolean CONTROLLER_SUPPORT;
	static GLboolean KEYBOARD_SUPPORT;

	// Camera
	static GLfloat CAM_STANDARD_MIN_ZOOM;
	static GLfloat CAM_ZOOM_SPEED;
	static GLfloat CAM_MAX_ZOOM;

	// Game
	static GLuint PLAYER_AMOUNT;
	static glm::vec3 PLAYER_COLORS[4];

	// Physics constants
	static GLfloat GRAV_ACC;
	static GLfloat COEFFFICIENT_OF_RESTITUTION; // The amount of elastic restitution. 1 = elastic collision | 0 = inelastic constitution

	// Rendering
	static GLboolean FULLSCREEN;
	static GLuint SCREEN_WIDTH; // Initial screen diementions
	static GLuint SCREEN_HEIGHT;

	// Other
	static GLfloat CONTROLLER_STICK_MAX; // The maximum value the stick can return used to normalize values
	static GLfloat CONTROLLER_TRIGGER_MAX; // The maximum value the trigger can return -==-
	static GLfloat CONTROLLER_DEADZONE; // The deadzone is an interval of values that the controller could return even if the analog sticks are not used

	// Functions
	static glm::mat2 RotationMat2(GLfloat angle); // Creates a matrix that rotates a 2D vector counter clockwise by a given angle in radians

	static void PrintVec2(glm::vec2 v);
};


// Debug
//#define DEBUG
#ifdef DEBUG
//#define LOG_FPS
#define DEBUG_HITBOXES
#define DEBUG_FORCES
#define LOG(x) std::cout << x << std::endl
#endif

#ifdef DEBUG_FORCES
#define FORCE_SCALE  0.001f
#endif // DEBUG_FORCES