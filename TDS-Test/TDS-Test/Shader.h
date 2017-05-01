#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint Program;

	Shader() {}

	void Use(); // sets the current shader as active

	void Compile(const GLchar *vertexSource, const GLchar *fragmentSource);

private:
	void checkCompileErrors(GLuint object, std::string type);
};

