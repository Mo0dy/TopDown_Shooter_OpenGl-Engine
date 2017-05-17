#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity() {}
Entity::Entity(Etex *etex, GLfloat width) : pos(0), angle(0), tex(etex->GetTex()), hitObjs(etex->GetHitObjs(width)), size(etex->GetTexSize()) {}
Entity::Entity(glm::vec2 position) : pos(position), angle(0) {}
Entity::Entity(glm::vec2 position, GLfloat angle) : pos(position), angle(angle) {}
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
		hitObjs = animations[ani].GetETex().GetHitObjs();
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

void Entity::GetAttacked(GLfloat damage)
{
}

// Getters and setters
void Entity::SetColor(glm::vec3 color) { this->color = color; }

GLboolean Entity::GetErase() const { return this->erase; }
glm::vec2 Entity::GetPos() const { return this->pos; }
glm::vec2 Entity::GetSize() const { return this->size; }
GLfloat Entity::GetAngle() const { return this->angle; }
glm::vec3 Entity::GetColor() const { return this->color;  }
const Texture2D* Entity::GetTex() const { return this->tex; }

void Entity::SetSize(glm::vec2 size) { this->size = size; }
void Entity::SetSize(GLfloat width) 
{
	this->size = glm::vec2(width, this->tex->Height / this->tex->Width * width);
}