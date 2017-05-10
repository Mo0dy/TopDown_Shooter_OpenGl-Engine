#pragma once

#include "Util.h"
#include "Texture2D.h"
#include "ResourceManager.h"

struct Etex {
public:
	Etex(std::string texture, GLfloat width);
	std::string tex;
	glm::vec2 texSize;
	std::vector<Hitbox*> hitboxes;
};

class Animation
{
public:

	static std::map<std::string, std::vector<Etex*> > Animations;

	Animation(std::string name, GLboolean repeat);
	~Animation();

	static void Animation::LoadAnimation(std::string path, std::string filetype, GLint amount, GLfloat width, GLboolean alpha, std::string name);

	void startAnimation();
	void stopAnimation();

	void setFPS(GLfloat fps);

	Etex* getETex();
	GLint getSize();
	GLboolean getState();
	GLfloat animationTime;
protected:
	GLboolean repeat;
	GLboolean state;
	GLfloat startTime;

	std::string name;
};

