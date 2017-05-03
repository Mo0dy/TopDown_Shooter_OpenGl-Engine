#pragma once

#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#define LOG(x) std::cout << x << std::endl;

static const float GRAV_ACC = 9.81f;

static void printVec2(glm::vec2 v) {
	std::cout << "[" << v.x << ", " << v.y << "]" << std::endl;
}