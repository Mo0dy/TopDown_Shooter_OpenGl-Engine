#pragma once

#include "Util.h"
#include "Texture2D.h"

class Etex {
public:

	Etex();
	Etex(Texture2D* texture, GLfloat width);
	virtual ~Etex();
	Texture2D* getTex();

	void fitHitboxToTex();

	glm::vec2 getTexSize();
	void setTexSize(glm::vec2 textureSize);
	void setTexSize(GLfloat width);

	std::vector<Hitbox*> getAbsHitboxes();
	void setRelHitboxes(std::vector<rHitbox*> Hboxes);
	void addRelHitbox(rHitbox* Hbox);

	void fitTex(GLfloat width);

	Texture2D* tex;

	std::vector<Hitbox*> absHitboxes;

	void updateAbsHitboxes();

protected:
	std::vector<rHitbox*> hitboxes;
	glm::vec2 texSize;

	void delHitboxes();
	void delAbsHitboxes();
};

