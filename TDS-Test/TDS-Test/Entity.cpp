#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity() : pos(glm::vec2(0)), angle(0), color(1), etex(Etex()) {
}
Entity::Entity(glm::vec2 position) : pos(position), angle(0), color(1), etex(Etex()) {
}
Entity::Entity(glm::vec2 position, Etex etex) : pos(position), angle(0), color(1), etex(etex) {
}
Entity::Entity(glm::vec2 position, GLfloat angle, Etex etex) : pos(position), angle(angle), color(1), etex(etex) {
}
Entity::Entity(glm::vec2 position, Animation ani, std::string aniName) : pos(position), angle(0), color(1), ani(aniName), etex(ani.getETex(0)) {
	Animations[aniName] = ani;
}
Entity::Entity(glm::vec2 position, GLfloat angle, Animation ani, std::string aniName) : pos(position), angle(angle), color(1), ani(aniName), etex(ani.getETex(0)) {
	Animations[aniName] = ani;
}

Entity::~Entity()
{
}

GLboolean Entity::updateE(GLfloat dt) {
	if (Animations[ani].getSize() > 0) {
		updateAni();
	}
	return GL_FALSE; // static entities never move
}

GLboolean Entity::checkForErase(glm::vec2 levelSize) {
	if (pos.x > levelSize.x * 0.5f || pos.x < -levelSize.x * 0.5f || pos.y > levelSize.y * 0.5f || pos.y < -levelSize.y * 0.5f) {
		return GL_TRUE;
	}
	else {
		return GL_FALSE;
	}
}

void Entity::setColor(glm::vec3 color) {
	this->color = color;
}

void Entity::updateAni() {
	etex = Animations[ani].getETex();
}

void Entity::updateHitboxes() {
	Hitboxes = etex.getAbsHitboxes();
}