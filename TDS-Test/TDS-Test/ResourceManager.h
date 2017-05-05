#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include "shader.h"
#include "Texture2D.h"
#include "myAnimation.h"

class ResourceManager
{
public:

	static std::map<std::string, Shader> Shaders; // hashmap containing shaders
	static std::map<std::string, Texture2D> Textures; // hashmap containing textures
	static std::map<std::string, myAnimation> Animations;

	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name); // Loads Shader from file into hashmap under "name"
	static Shader GetShader(std::string name);

	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name); // Loads Texture from file into hasmap under "name"
	static Texture2D GetTexture(std::string name);

	static myAnimation LoadAnimation(std::string path, std::string filetype, GLint amount, GLfloat width, GLboolean alpha, std::string name); // Loads Texture from file into hasmap under "name"
	static myAnimation getAnimation(std::string name);

	static void Clear(); // deletes all Shaders and Textures

private:
	ResourceManager() {}

	// Loads and generates a shader from file
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
	static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);

};

