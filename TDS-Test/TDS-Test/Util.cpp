#pragma once

#include "Util.h"

// Settings
// Controles
GLboolean Util::CONTROLLER_SUPPORT = GL_TRUE;
GLboolean Util::KEYBOARD_SUPPORT = GL_TRUE;

// Camera
GLfloat Util::CAM_STANDARD_MIN_ZOOM = 10;
GLfloat Util::CAM_ZOOM_SPEED = 0.2;
#ifdef DEBUG
GLfloat Util::CAM_MAX_ZOOM = 120;
#else // !DEBUG
GLfloat Util::CAM_MAX_ZOOM = 25;
#endif // !DEBUG

// Game
GLuint Util::PLAYER_AMOUNT = 1;
glm::vec3 Util::PLAYER_COLORS[] = {
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f),
	glm::vec3(1.0f)
};

// Physics constants
GLfloat Util::GRAV_ACC = 9.81f;
GLfloat Util::COEFFFICIENT_OF_RESTITUTION = 1;

// Rendering
GLboolean Util::FULLSCREEN = GL_TRUE;
GLuint Util::SCREEN_WIDTH = 1920;
GLuint Util::SCREEN_HEIGHT = 1080;

// Other
GLfloat Util::CONTROLLER_STICK_MAX = 32767.0f;
GLfloat Util::CONTROLLER_TRIGGER_MAX = 255.0f;
GLfloat Util::CONTROLLER_DEADZONE = 6500.0f;

// utility funciton
glm::mat2 Util::RotationMat2(GLfloat angle) {
	glm::mat2 rotMat;
	rotMat[0][0] = cos(angle);
	rotMat[1][0] = -sin(angle);
	rotMat[0][1] = sin(angle);
	rotMat[1][1] = cos(angle);
	return rotMat;
}

void Util::PrintVec2(glm::vec2 v) {
	std::cout << "[" << v.x << ", " << v.y << "]" << std::endl;
}

GLfloat Util::CalcAbsAngle(glm::vec2 v) 
{
	v = glm::normalize(v);
	return glm::mod<GLfloat>((glm::pi<GLfloat>() * 2 + glm::acos(v.y) * v.x / glm::abs(v.x)), glm::pi<GLfloat>() * 2);
}
