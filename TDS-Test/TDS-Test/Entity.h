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

	void initRenderData();
	GLuint const getVAO();
	std::string const getTexture();
	glm::vec2 pos;
	GLfloat angle;
private:
	std::string myTexture;
	GLuint quadVAO;
};

