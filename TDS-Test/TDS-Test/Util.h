#pragma once

#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>

// Debug
#define DEBUG
#ifdef DEBUG
	#define DEBUG_FORCES
	#define LOG(x) std::cout << x << std::endl;
#endif

// Physics
static const GLfloat GRAV_ACC = 9.81f;

// Sudo Physics
static const GLfloat COLLISION_ADD_CHANGE = 0.5f;

// Rendering
static const GLfloat FORCE_SCALE = 0.001f;

static void printVec2(glm::vec2 v) {
	std::cout << "[" << v.x << ", " << v.y << "]" << std::endl;
}