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
#include "Constants.h"

class Entity
{
public:
	Entity(std::string texture); // The name under which the corresponding texture was loaded into the RecourceManager
	virtual ~Entity();
	std::string tex; // The hash of the texture stored in the Resource
	glm::vec2 size; // The size of the entities sprite in WCO coordinates (rectangle with the diagonal of size)
	glm::vec2 pos; // The position in WCS coordinates
	GLfloat angle; // The angle of the entitie in degrees
	
protected:
};

