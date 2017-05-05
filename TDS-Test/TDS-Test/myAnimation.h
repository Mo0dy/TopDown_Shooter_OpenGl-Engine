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
	Etex(std::string texture, GLfloat width): tex(texture) {
		texSize.x = width;
		texSize.y = width * ResourceManager::GetTexture(texture).Height / ResourceManager::GetTexture(texture).Width;
	}
	std::string tex;
	glm::vec2 texSize;
	std::vector<Hitbox> hitboxes;
};

class myAnimation
{
public:
	myAnimation();
	~myAnimation();

	void startAnimation();
	void stopAnimation();

	Etex* getETex();
	void addETex(Etex*);
	GLint getSize();
	GLboolean getState();
protected:
	GLboolean state;
	GLfloat startTime;
	GLfloat animationTime;
	std::vector<Etex*> textures;
};

