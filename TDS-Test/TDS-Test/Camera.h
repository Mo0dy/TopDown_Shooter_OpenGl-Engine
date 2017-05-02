#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera();
	~Camera();
	void updatePos(float width, float height);
	glm::vec2 pos;
	glm::vec2 size;
private:
};

