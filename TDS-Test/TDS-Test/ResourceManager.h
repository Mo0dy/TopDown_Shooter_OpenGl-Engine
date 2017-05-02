#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include "shader.h"
#include "Texture2D.h"

class ResourceManager
{
public:

	static std::map<std::string, Shader> Shaders; // hashmap containing shaders
	static std::map<std::string, Texture2D> Textures; // hashmap containing textures

	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name); // Loads Shader from file into hashmap under "name"
	static Shader GetShader(std::string name);

	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name); // Loads Texture from file into hasmap under "name"
	static Texture2D GetTexture(std::string name); 

	static void Clear(); // deletes all Shaders and Textures

private:
	ResourceManager() {}

	// Loads and generates a shader from file
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);

};

