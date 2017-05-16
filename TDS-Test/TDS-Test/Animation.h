#pragma once

#include "Util.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include "Etex.h"

class Animation
{
public:
	Animation();
	Animation(std::string name, GLboolean repeat);
	Animation(std::string name, GLfloat width, GLboolean repeat);
	~Animation();

	void Start();
	void Stop();

	void LoadFromRM(std::string name);
	void LoadFromRM(std::string name, GLfloat width);

	void EnforceWidth(GLfloat width);

	// Getters and setters
	void SetFPS(GLfloat fps);
	void SetAniTime(GLfloat aniTime);

	Etex& GetETex();
	Etex& GetETex(GLuint pos);
	GLint GetNumber();
	GLboolean GetState();
	
	std::vector<Etex> Etextures;

protected:
	GLfloat aniTime;
	GLboolean repeat;
	GLboolean state;
	GLfloat startTime;
};

