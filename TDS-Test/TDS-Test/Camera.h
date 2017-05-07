#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include "Player.h"

static const GLfloat OFFSET = 2;

class Camera
{
public:
	Camera();
	~Camera();
	void updatePos(GLfloat width, GLfloat height, std::vector<Player*>);
	glm::vec2 pos; // Bottom left coordinate of the Camera viewspace in WCS coordinates
	glm::vec2 size; // The bigger the camera the smaller everything else (linear) in WCS coordinates

	GLfloat minSizeHeight;

private:
};

