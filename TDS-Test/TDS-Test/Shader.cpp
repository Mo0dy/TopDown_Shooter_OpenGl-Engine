#include "Shader.h"
#include <iostream>
#include "ResourceManager.h"

void Shader::Use() {
	glUseProgram(Program);
}

void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentSource) {
	GLuint sVertex, sFragment;
	// Vertex Shader
	sVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(sVertex, 1, &vertexSource, NULL);
	glCompileShader(sVertex);
	checkCompileErrors(sVertex, "VERTEX");
	// Fragment Shader
	sFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(sFragment, 1, &fragmentSource, NULL);
	glCompileShader(sFragment);
	checkCompileErrors(sFragment, "FRAGMENT");
	// Shader Program
	Program = glCreateProgram();
	glAttachShader(Program, sVertex);
	glAttachShader(Program, sFragment);
	glLinkProgram(Program);
	checkCompileErrors(Program, "PROGRAM");
	glDeleteShader(sVertex);
	glDeleteShader(sFragment);
}

void Shader::SetVector3f(const GLchar *name, const glm::vec3 &value)
{
	Use();
	glUniform3f(glGetUniformLocation(Program, name), value.x, value.y, value.z);
}

void Shader::SetMatrix4(const GLchar *name, const glm::mat4 &matrix) {
	Use();
	glUniformMatrix4fv(glGetUniformLocation(Program, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint object, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(object, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
	else
	{
		glGetProgramiv(object, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(object, 1024, NULL, infoLog);
			std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
				<< infoLog << "\n -- --------------------------------------------------- -- "
				<< std::endl;
		}
	}
}


