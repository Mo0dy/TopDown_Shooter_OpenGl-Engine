#include "HitObject.h"

HitObject::HitObject() {}
HitObject::HitObject(glm::vec2 pos, glm::vec2 size, GLfloat angle) : pos(pos), size(size), angle(angle) {}

HitObject::~HitObject()
{
}

// Getters and setters
glm::vec2 HitObject::GetPos()
{
	return pos;
}
glm::vec2 HitObject::GetSize()
{
	return size;
}
GLfloat HitObject::GetAngle()
{
	return angle;
}

void HitObject::SetPos(glm::vec2 pos)
{
	this->pos = pos;
}
void HitObject::SetSize(glm::vec2 size)
{
	this->size = size;
}
void HitObject::SetAngle(GLfloat angle)
{
	this->angle = angle;
}