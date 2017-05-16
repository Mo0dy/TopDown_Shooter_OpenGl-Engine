#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity() {}
Entity::Entity(glm::vec2 position) : pos(position) {}
Entity::Entity(glm::vec2 position, const Texture2D* texture) : pos(position), tex(texture) {}
Entity::Entity(glm::vec2 position, GLfloat angle, const Texture2D* texture) : pos(position), angle(angle), tex(texture) {}
Entity::Entity(glm::vec2 position, Animation ani, std::string aniName) : pos(position)
{
	animations[aniName] = ani;
}
Entity::Entity(glm::vec2 position, GLfloat angle, Animation ani, std::string aniName) : pos(position), angle(angle)
{
	animations[aniName] = ani;
}

Entity::~Entity() {}

GLboolean Entity::UpdateE(GLfloat dt) 
{
	if (animations[ani].GetNumber() > 0) {
		UpdateAni();
	}
	return GL_FALSE; // static entities never move
}

void Entity::UpdateAni() 
{
	if (animations[ani].GetState()) {
		tex = animations[ani].GetETex().GetTex();
		hitboxes = animations[ani].GetETex().GetHitObjs();
	}
}

void Entity::Collision(const Entity* e, GLfloat penDepth, glm::vec2 colAxis)
{

}
void Entity::ColWithStat(const Entity* e, GLfloat penDepth, glm::vec2 colAxis)
{

}
void Entity::ColWithDyn(const class DynE* dE, GLfloat penDepth, glm::vec2 colAxis)
{

}
void Entity::ColWithEnemy(const class Enemy* e, GLfloat penDepth, glm::vec2 colAxis)
{

}
void Entity::ColWithPlayer(const class Player* p, GLfloat penDepth, glm::vec2 colAxis)
{

}

// Getters and setters
void Entity::SetColor(glm::vec3 color) { this->color = color; }

GLboolean Entity::GetErase() { return erase; }
glm::vec2 Entity::GetPos() { return pos; }
GLfloat Entity::GetAngle() { return angle; }
glm::vec3 Entity::GetColor() { return color;  }