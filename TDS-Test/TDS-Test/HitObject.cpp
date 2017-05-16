#include "HitObject.h"

HitObject::HitObject() {}
HitObject::HitObject(glm::vec2 pos, glm::vec2 size, GLfloat angle) : pos(pos), size(size), angle(angle) {}

HitObject::~HitObject()
{
}
