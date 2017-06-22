#pragma once

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	GLuint Program; // The ID of the shader Program

	Shader() {}
	void Use(); // sets the current shader as active
	void Compile(const GLchar *vertexSource, const GLchar *fragmentSource);

	// Utility functions add more if you need them
	void SetVector3f(const GLchar *name, const glm::vec3 &value);
	void SetMatrix4(const GLchar *name, const glm::mat4 &matrix); // Sets matrix4 Uniform "name" to matrix
	void SetInt(const GLchar *name, const GLuint ID);

private:
	void checkCompileErrors(GLuint object, std::string type);
};

