#include "Entity.h"
#include "ResourceManager.h"
#include "SubE.h"

Entity::Entity() {}

Entity::Entity(glm::vec2 position) : pos(position, 1.0f), angle(0), color(glm::vec3(1)) {}
Entity::Entity(glm::vec2 position, GLfloat angle) : pos(position, 1.0f), angle(angle), color(glm::vec3(1)) {}
Entity::Entity(glm::vec2 position, const Texture2D* texture) : pos(position, 1.0f), tex(texture), color(glm::vec3(1)) {}
Entity::Entity(glm::vec2 position, GLfloat angle, const Texture2D* texture) : pos(position, 1.0f), angle(angle), tex(texture), color(glm::vec3(1)) {}
Entity::Entity(glm::vec2 position, Animation ani, std::string aniName) : pos(position, 1.0f), color(glm::vec3(1)) { animations[aniName] = ani; }
Entity::Entity(glm::vec2 position, GLfloat angle, Animation ani, std::string aniName) : pos(position, 1.0f), angle(angle), color(glm::vec3(1)) { animations[aniName] = ani; }
Entity::Entity(const Etex *etex, GLfloat width, GLfloat height) : pos(glm::vec2(0), 1.0f), angle(0), tex(etex->GetTex()), hitComb(etex->GetHitComb(width)), size(etex->GetTexSize(width), height), color(glm::vec3(1)) {}
Entity::Entity(const Etex* etex, glm::vec3 size) : pos(glm::vec2(0), 1.0f), angle(0), tex(etex->GetTex()), hitComb(etex->GetHitComb(size)), size(size), color(glm::vec3(1)) {}

Entity::~Entity() {}

GLboolean Entity::UpdateE(GLfloat dt) 
{
	if (animations[ani].GetNumber() > 0) {
		UpdateAni();
	}
	return GL_FALSE; // static entities never move
}

void Entity::UpdateAni() { animations[ani].UpdateAni(this, GL_FALSE); }
void Entity::Collision(Entity* e, GLfloat penDepth, glm::vec2 colAxis) {}
void Entity::ColWithStat(Entity* e, GLfloat penDepth, glm::vec2 colAxis) {}
void Entity::ColWithDyn(class DynE* dE, GLfloat penDepth, glm::vec2 colAxis) {}
void Entity::ColWithEnemy(class Enemy* e, GLfloat penDepth, glm::vec2 colAxis) {}
void Entity::ColWithPlayer(class Player* p, GLfloat penDepth, glm::vec2 colAxis) {}
void Entity::ColWithSubE(class SubE* sE, GLfloat penDepth, glm::vec2 colAxis) { ColWithDyn(sE->masterE, penDepth, colAxis); }
void Entity::ColWithESubE(class SubE* e, GLfloat penDepth, glm::vec2 colAxis) { ColWithDyn(e->masterE, penDepth, colAxis); }
void Entity::ColWithPSubE(class SubE* p, GLfloat penDepth, glm::vec2 colAxis) { ColWithDyn(p->masterE, penDepth, colAxis); }

void Entity::GetAttacked(GLfloat damage) {}

// Getters and setters
GLboolean Entity::GetErase() const { return this->erase; }
glm::vec2 Entity::Get2DPos() const { return glm::vec2(this->pos.x, this->pos.y); }
glm::vec2 Entity::Get2DSize() const { return glm::vec2(this->size.x, this->size.y); }
glm::vec3 Entity::GetPos() const { return this->pos; }
glm::vec3 Entity::GetSize() const { return this->size; }
GLfloat Entity::GetZPos() const { return this->pos.z; }
GLfloat Entity::GetZSize() const { return this->size.z; }
GLfloat Entity::GetAngle() const { return this->angle; }
glm::vec3 Entity::GetColor() const { return this->color;  }
const Texture2D* Entity::GetTex() const { return this->tex; }
HitComb Entity::GetHitComb() const { return this->hitComb; }

void Entity::SetColor(glm::vec3 color) { this->color = color; }
void Entity::SetTex(const Texture2D *tex) { this->tex = tex; }
void Entity::SetSize(glm::vec2 size) { this->size = glm::vec3(size, this->size.z); }
void Entity::SetSize(GLfloat width) { this->size = glm::vec3(width, (GLfloat) this->tex->Height / this->tex->Width * width, this->size.z); }
void Entity::SetSize(glm::vec3 size) { this->size = size; }
void Entity::SetZSize(GLfloat zSize) { this->size.z = zSize; }
void Entity::SetHitComb(HitComb hitComb) { this->hitComb = hitComb; }