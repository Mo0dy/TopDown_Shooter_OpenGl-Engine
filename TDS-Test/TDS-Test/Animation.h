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
	Animation();
	~Animation();

	void Animation::LoadAnimation(std::string path, std::string filetype, GLint amount, GLfloat width, GLboolean alpha, std::string name);

	void startAnimation();
	void stopAnimation();

	Etex* getETex();
	void addETex(Etex*);
	GLint getSize();
	GLboolean getState();
	GLfloat animationTime;
protected:
	GLboolean state;
	GLfloat startTime;
	std::vector<Etex*> textures;
};

