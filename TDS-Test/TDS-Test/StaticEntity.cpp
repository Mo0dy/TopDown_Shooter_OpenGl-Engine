#include "StaticEntity.h"

StaticEntity::StaticEntity() {
}
StaticEntity::StaticEntity(glm::vec2 position, GLfloat width, GLfloat angle, Animation ani, std::string aniName, GLboolean concealing) : Entity(position, angle, ani, aniName), concealing(concealing) {
	Animations[this->ani].enforceWidth(width);
}
StaticEntity::StaticEntity(glm::vec2 position, GLfloat width, GLfloat angle, Etex etex, GLboolean concealing) : Entity(position, angle, etex), concealing(concealing) {
	etex.setTexSize(width);
}

StaticEntity::~StaticEntity()
{
}
