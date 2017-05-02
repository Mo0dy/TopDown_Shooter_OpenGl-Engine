#pragma once

#define GLEW_STATIC // GLEW
#include <GL/glew.h>
#include <GLFW/glfw3.h> // GLFW
#include <SOIL/SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include "Texture2D.h"
#include "Shader.h"
#include "Texture2D.h"

class Entity
{
public:
	Entity(std::string texture);
	~Entity();

	std::string myTexture;
	glm::vec2 size;
	glm::vec2 pos;
	GLfloat angle;
	
private:
};

