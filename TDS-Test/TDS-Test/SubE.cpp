#include "SubE.h"

SubE::SubE(glm::vec2 masterPos, glm::vec2 relativePos): Entity(masterPos + relativePos), rPos(relativePos), rAngle(0)
{
}

SubE::~SubE()
{
}
