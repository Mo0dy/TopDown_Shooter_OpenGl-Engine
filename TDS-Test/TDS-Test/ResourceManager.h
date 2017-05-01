#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include "shader.h"

class ResourceManager
{
public:

	static std::map<std::string, Shader> Shaders;
	static void LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name);
	static Shader GetShader(std::string name);

	static void Clear();

private:
	ResourceManager() {}

	// Loads and generates a shader from file
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
};

