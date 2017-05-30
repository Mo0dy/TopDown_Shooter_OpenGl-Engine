#pragma once

#include "Util.h"
#include "Camera.h"
#include "HitObject.h"

class SightCalc
{
public:
	SightCalc();
	~SightCalc();

	void CalcObs(std::vector<Entity*> viewers, std::vector<Entity*> obstuctors, Camera* cam);

private:
	std::vector<glm::vec2> sightVertices;

	std::vector<glm::vec2> SortForAngle(std::vector<glm::vec2> vertices);
	GLboolean CompareVectors(glm::vec2 v1, glm::vec2 v2); // returns true if v1 has a bigger angle then v2
};

