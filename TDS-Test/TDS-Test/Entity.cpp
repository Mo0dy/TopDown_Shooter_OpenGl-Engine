#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity() : tex("Util"), pos(glm::vec2(0)), angle(0), size(1, 1), color(1.0f, 1.0f, 1.0f) {
}

Entity::Entity(glm::vec2 position) : tex("Util"), pos(position), angle(0), size(1, 1), color(1.0f, 1.0f, 1.0f)
{
}

Entity::Entity(glm::vec2 position, glm::vec2 size) : tex("Util"), pos(position), angle(0), size(size), color(1.0f, 1.0f, 1.0f)
{
}

Entity::Entity(glm::vec2 position, glm::vec2 size, GLfloat angle) : tex("Util"), pos(position), angle(angle), size(size), color(1.0f, 1.0f, 1.0f) {
}

Entity::Entity(glm::vec2 position, glm::vec2 size, std::string texture) : tex(texture), pos(position), angle(0), size(size), color(1.0f, 1.0f, 1.0f) {
}

Entity::Entity(glm::vec2 position, GLfloat width, std::string texture) : tex(texture), pos(position), angle(0), size(glm::vec2(width, width * ResourceManager::GetTexture(texture).Height / ResourceManager::GetTexture(texture).Width)), color(1.0f, 1.0f, 1.0f) {
}

Entity::Entity(glm::vec2 position, GLfloat width, GLfloat angle, std::string texture) : tex(texture), pos(position), angle(angle), size(glm::vec2(width, width * ResourceManager::GetTexture(texture).Height / ResourceManager::GetTexture(texture).Width)), color(1.0f, 1.0f, 1.0f) {
}

Entity::Entity(glm::vec2 position, glm::vec2 size, GLfloat angle, std::string texture) : tex(texture), pos(position), angle(angle), size(size), color(1.0f, 1.0f, 1.0f) {
}

Entity::~Entity()
{
	for (Hitbox* h : Hitboxes) {
		delete h;
	}
	for (const auto& a : Animations) {
		delete a.second;
	}
}

GLboolean Entity::updateE(GLfloat dt) {
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

void Entity::autofitHitbox() {
	Hitboxes.push_back(new Hitbox(glm::vec2(0), size, 0));
}