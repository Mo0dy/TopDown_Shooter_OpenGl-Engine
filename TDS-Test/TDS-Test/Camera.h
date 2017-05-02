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
	void updatePos(float width, float height, glm::vec2 goalPos); // goalPos is a placeholder until we can write a proper updateFunction that depends on the player positions then those will be passed as std::vector<*Player>
	glm::vec2 pos; // Bottom left coordinate of the Camera viewspace in WCS coordinates
	glm::vec2 size; // The bigger the camera the smaller everything else (linear) in WCS coordinates
private:
};

