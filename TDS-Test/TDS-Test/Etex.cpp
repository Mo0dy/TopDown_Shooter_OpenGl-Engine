#include "Etex.h"
#include "ResourceManager.h"

Etex::Etex(): tex(new Texture2D()){
}

Etex::Etex(Texture2D* texture, GLfloat width) : tex(texture) {
	fitTex(width);
}

Etex::~Etex()
{
	// This should really be deleted
	//delHitboxes();
	//delAbsHitboxes();
}

std::vector<Hitbox*> Etex::getAbsHitboxes() {
	return absHitboxes;
}

void Etex::setRelHitboxes(std::vector<rHitbox*> Hboxes) {
	delHitboxes();
	hitboxes = Hboxes;
}

void Etex::addRelHitbox(rHitbox* Hbox) {
	hitboxes.push_back(Hbox);
}

void Etex::fitTex(GLfloat width) {
	texSize.x = width;
	texSize.y = width * tex->Height / tex->Width;
}

void Etex::fitHitboxToTex() {
	hitboxes.clear();
	hitboxes.push_back(new rHitbox(glm::vec2(0), glm::vec2(1), 0));
	updateAbsHitboxes();
}

void Etex::updateAbsHitboxes() {
	delAbsHitboxes();
	absHitboxes.clear();
	for (rHitbox* rH : hitboxes) {
		absHitboxes.push_back(new Hitbox(rH->rPosProp * texSize * 0.5f, rH->rSizeProp * texSize, rH->rAngle));
	}
}

void Etex::delHitboxes() {
	for (rHitbox* h : hitboxes) {
		delete h;
	}
}

void Etex::delAbsHitboxes() {
	// buggy for some reason
	//for (Hitbox* h : absHitboxes) {
	//	delete h;
	//}
}

void Etex::setTexSize(glm::vec2 textureSize) {
	texSize = textureSize;
	updateAbsHitboxes();
}

void Etex::setTexSize(GLfloat width) {
	setTexSize(glm::vec2(width, width * tex->Height / tex->Width));
}

Texture2D* Etex::getTex() { return tex; }

glm::vec2 Etex::getTexSize() { return texSize; }