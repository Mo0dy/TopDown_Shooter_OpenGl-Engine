#include "StaticEntity.h"

StaticEntity::StaticEntity() {
}
StaticEntity::StaticEntity(glm::vec2 position, GLfloat width, GLfloat angle, Animation ani, std::string aniName, GLboolean concealing) : Entity(position, angle, ani, aniName), concealing(concealing) {
	animations[this->ani].EnforceWidth(width);
}
StaticEntity::StaticEntity(glm::vec2 position, GLfloat width, GLfloat angle, Etex etex, GLboolean concealing) : Entity(&etex, width), concealing(concealing) {
	// This should be done in an entity constructor
	this->pos = position;
	this->angle = angle;
}

StaticEntity::~StaticEntity()
{
}
