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
	void updatePos();
	glm::vec3 getPos();
private:
	glm::vec3 pos;
};

