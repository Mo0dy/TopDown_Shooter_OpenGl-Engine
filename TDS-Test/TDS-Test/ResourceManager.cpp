#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <SOIL\SOIL.h>

// Instantiate static variables
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Etex> ResourceManager::Etextures;
std::map<std::string, std::vector<Etex> > ResourceManager::Animations;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, std::string name) {
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) {
	return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = *loadTextureFromFile(file, alpha);
	return Textures[name];
}

Texture2D ResourceManager::GetTexture(std::string name) {
	return Textures[name];
}

Etex ResourceManager::LoadEtex(std::string path, std::string filename, std::string filetype, GLboolean alpha, std::string name, LOAD_HBOX_SWITCH loadHbox) {
	Etex* etex = new Etex;
	LoadTempEtex(path, filename, filetype, alpha, loadHbox, etex); // Problemstelle ==================================================================
	Etextures[name] = *etex;
	return Etextures[name];
}

void ResourceManager::LoadTempEtex(std::string path, std::string filename, std::string filetype, GLboolean alpha, LOAD_HBOX_SWITCH loadHbox, Etex* etexToFill) {
	etexToFill->SetTex(loadTextureFromFile((path + "\\T" + filename + filetype).c_str(), alpha));
	if (loadHbox == HBOX_LOAD_ONE) {
		etexToFill->SetRHComb(loadrHitboxFromFile((path + "\\H" + filename + ".txt").c_str()));
	}
	else if (loadHbox == HBOX_AUTOFIT) {
		etexToFill->FitHObj();
	}
}

Etex ResourceManager::GetEtex(std::string name) { return Etextures[name]; }

void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto s : Shaders)
		glDeleteProgram(s.second.Program);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.Tex);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile) {
	std::string vertexCode;
	std::string fragmentCode;

	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode);
	return shader;
}

Texture2D* ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	Texture2D *texture = new Texture2D();
	if (alpha)
	{
		texture->Internal_Format = GL_RGBA;
		texture->Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture->Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture->Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}

HitComb* ResourceManager::loadrHitboxFromFile(const char* path) {
	std::string hitboxConfig;

	HitComb* hitComb = new HitComb;

	try
	{
		// Open files
		std::ifstream hitboxConfigFile(path);
		std::stringstream vHitboxStream;
		// Read file's buffer contents into streams
		vHitboxStream << hitboxConfigFile.rdbuf();
		// close file handlers
		hitboxConfigFile.close();
		// Convert stream into string
		hitboxConfig = vHitboxStream.str();

		std::vector<std::string> param(1);

		for (char& c : hitboxConfig) {
			if (c == ',') {
				param.push_back("");
			}
			else if (c == ';') {
				param.push_back(";");
				param.push_back("");
			}
			else {
				if (c != '\n' && c != ' ') {
					param.back() += c;
				}
			}
		}

		for (int i = 0; i < param.size(); i++) {
			if (param[i] == "HB") {
				i++;
				hitComb->hitBoxes.push_back(HitBox(glm::vec2(strtof(param[i].c_str(), 0), strtof(param[i + 1].c_str(), 0)), glm::vec2(strtof(param[i + 2].c_str(), 0), strtof(param[i + 3].c_str(), 0)), glm::radians(strtof(param[i + 4].c_str(), 0))));
				i += 5;
			}
			else if (param[i] == "HP") {
				i++;
				hitComb->hitPolys.push_back(HitPoly());
				while (i < param.size() && param[i] != ";") {
					hitComb->hitPolys.back().AddVertex(glm::vec2(strtof(param[i].c_str(), 0), strtof(param[i + 1].c_str(), 0)));
					i += 2;
				};
				hitComb->hitPolys.back().Update();
			}
			else if (param[i] == "HC") {
				i++;
				hitComb->hitCircles.push_back(HitCircle(glm::vec2(strtof(param[i].c_str(), 0), strtof(param[i + 1].c_str(), 0)), strtof(param[i + 2].c_str(), 0)));
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::HITBOX: Failed to read hitbox file" << std::endl;
	}

	return hitComb;
}

// The Etextures probably shouldn't be saved in the rescource manager ?
void ResourceManager::LoadAnimation(std::string path, std::string filetype, GLint amount, GLfloat width, GLboolean alpha, std::string name, LOAD_ANIMATION_SWITCH loadHitboxes) {
	HitComb* tHComb = new HitComb;
	if (loadHitboxes == ANI_LOAD_ONE_HBOX) {
		tHComb = loadrHitboxFromFile((path + "\\H.txt").c_str());
	}

	LOAD_HBOX_SWITCH loadHbox;
	if (loadHitboxes == ANI_LOAD_ALL_HBOX) {
		loadHbox = HBOX_LOAD_ONE;
	}
	else {
		loadHbox = HBOX_LOAD_NONE;
	}
	for (int i = 0; i < amount; i++) {
		Etex* etex = new Etex;

		LoadTempEtex(path, std::to_string(i), filetype, alpha, loadHbox, etex);
		Animations[name].push_back(*etex);
		Animations[name].back().SetTexSize(width);

		if (loadHitboxes == ANI_LOAD_ONE_HBOX) {
			Animations[name].back().SetRHComb(tHComb);
		}
		else if (loadHitboxes == ANI_HBOX_AUTOFIT) {
			Animations[name].back().FitHObj();
		}
	}
}

std::vector<Etex> ResourceManager::GetAnimation(std::string name) {
	return Animations[name];
}