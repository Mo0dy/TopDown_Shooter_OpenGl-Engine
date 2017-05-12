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

	void startAnimation();
	void stopAnimation();

	void setFPS(GLfloat fps);

	void load(std::string name);
	void load(std::string name, GLfloat width);

	void enforceWidth(GLfloat width);

	GLfloat width;

	// Getters and setters
	Etex& getETex();
	Etex& getETex(GLuint pos);
	GLint getSize();
	GLboolean getState();
	GLfloat animationTime;
	
	std::vector<Etex> Etextures;

protected:
	GLboolean repeat;
	GLboolean state;
	GLfloat startTime;
};

