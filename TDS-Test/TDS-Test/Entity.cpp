#include "Entity.h"
#include "ResourceManager.h"

Entity::Entity() {}
Entity::Entity(glm::vec2 position) : pos(position) {}
Entity::Entity(glm::vec2 position, Texture2D texture) : pos(position), tex(texture) {}
Entity::Entity(glm::vec2 position, GLfloat angle, Texture2D texture) : pos(position), angle(angle), tex(texture) {}
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
		tex = *animations[ani].GetETex().tex;
		hitboxes = animations[ani].GetETex().absHitboxes;
	}
}

// Getters and setters
void Entity::SetColor(glm::vec3 color) { this->color = color; }

GLboolean Entity::GetErase() { return erase; }
glm::vec2 Entity::GetPos() { return pos; }
GLfloat Entity::GetAngle() { return angle; }
glm::vec3 Entity::GetColor() { return color;  }