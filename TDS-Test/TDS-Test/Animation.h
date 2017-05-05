#pragma once

#include "Util.h"
#include "Texture2D.h"
#include "ResourceManager.h"

struct Hitbox {
	glm::vec2 pos; // lower left corner
	glm::vec2 size; // Size of the rectangle from lower left corner
	GLfloat angle; // angle relative to the corresponding texture
};

struct Etex {
public:
	Etex(std::string texture, GLfloat width);
	std::string tex;
	glm::vec2 texSize;
	std::vector<Hitbox> hitboxes;
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

