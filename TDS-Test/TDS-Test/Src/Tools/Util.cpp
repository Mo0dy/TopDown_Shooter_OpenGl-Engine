#pragma once

#include "Util.h"
#include "..\OpenGL\Renderer.h"

// Settings
// Controles
GLboolean Util::CONTROLLER_SUPPORT = GL_TRUE;
GLboolean Util::KEYBOARD_SUPPORT = GL_TRUE;

// Camera
GLfloat Util::CAM_STANDARD_MIN_ZOOM = 10;
GLfloat Util::CAM_ZOOM_SPEED = 60;
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
GLboolean Util::FULLSCREEN = GL_FALSE;
GLuint Util::SCREEN_WIDTH = 1600;
GLuint Util::SCREEN_HEIGHT = 900;

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

GLboolean Util::CalcIntersection(glm::vec2 pointA, glm::vec2 vecA, glm::vec2 pointB, glm::vec2 vecB, glm::vec2 &result)
{
	// Check if the vectors are paralell
	if (vecA.y == vecA.x * vecB.y / vecB.x) { return GL_FALSE; }

	// Calculate scalar Muliplicators e & f for intersection with: A + ea = B + fb
	GLfloat f;

	f = (vecA.x * (pointB.y - pointA.y) + vecA.y * (pointA.x - pointB.x)) / (vecB.x * vecA.y - vecB.y * vecA.x);

	result = pointB + f * vecB;
	return GL_TRUE;
}

void Util::DrawGrid()
{
	GLfloat lineXEnd = 50;
	GLfloat lineXBegin = -50;
	GLfloat lineYBegin = -25;
	GLfloat lineYEnd = 150;

	glm::vec3 color(0.7f, 0.7f, 0.7f);

	while (lineYBegin < lineYEnd) {
		Renderer::sDrawLineBuffer.push_back(myVertex(glm::vec2(lineXBegin, lineYBegin), color));
		Renderer::sDrawLineBuffer.push_back(myVertex(glm::vec2(lineXEnd, lineYBegin) ,color));
		lineYBegin += 1;
	}
}