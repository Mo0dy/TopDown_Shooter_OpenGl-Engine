#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

#include "shader.h"
#include "Texture2D.h"
#include "Etex.h"

#include <vector>

enum LOAD_ANIMATION_SWITCH {
	ANI_LOAD_NO_HBOX,
	ANI_LOAD_ONE_HBOX,
	ANI_LOAD_ALL_HBOX,
	ANI_HBOX_AUTOFIT
};

enum LOAD_HBOX_SWITCH {
	HBOX_LOAD_ONE,
	HBOX_LOAD_NONE,
	HBOX_AUTOFIT
};

class ResourceManager
{
public:

	static std::map<std::string, Shader> Shaders; // hashmap containing shaders
	static std::map<std::string, Texture2D> Textures; // hashmap containing textures
	static std::map<std::string, Etex> Etextures;
	static std::map<std::string, std::vector<Etex> > Animations;

	static Shader LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name); // Loads Shader from file into hashmap under "name"
	static Shader GetShader(std::string name);

	static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name); // Loads Texture from file into hasmap under "name"
	static Texture2D GetTexture(std::string name);

	static Etex LoadEtex(std::string path, std::string filename, std::string filetype, GLboolean alpha, std::string name, LOAD_HBOX_SWITCH loadHbox);
	static void LoadTempEtex(std::string path, std::string filename, std::string filetype, GLboolean alpha, LOAD_HBOX_SWITCH loadHbox, Etex* etexToFill);

	static Etex GetEtex(std::string name);

	static std::vector<HitComb*> ResourceManager::loadrHitboxFromFile(const char* path);

	static void LoadAnimation(std::string path, std::string filetype, GLint amount, GLfloat width, GLboolean alpha, std::string name, LOAD_ANIMATION_SWITCH loadHitboxes);

	static std::vector<Etex> GetAnimation(std::string name);

	static void Clear(); // deletes all Shaders and Textures

private:
	ResourceManager() {}

	// Loads and generates a shader from file
	static Shader    loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile);
	static Texture2D* loadTextureFromFile(const GLchar *file, GLboolean alpha);

};

