#pragma once

#include "..\Tools\Util.h"
#include "..\OpenGl\Camera.h"
#include "HitObject.h"

#include "..\GameContent\Entities\StaticEntity.h"

struct Line {
	Line();
	Line(glm::vec2 v1, glm::vec2 v2);

	GLfloat minAngle;
	GLfloat maxAngle;

	glm::vec2 v1;
	glm::vec2 v2;

	void CalcAngle();

	static void SortForMinAngle(std::vector<Line> &lines);
};

struct Ray {
	Ray();
	Ray(glm::vec2 dir);
	Ray(glm::vec2 dir, GLfloat angle);

	GLfloat angle;
	glm::vec2 dir;

	void CalcAngle();

	static void SortForAngle(std::vector<Ray> &rays);
};

class SightCalc
{
public:
	SightCalc();
	~SightCalc();

	void CalcObs(std::vector<Entity*> viewers, std::vector<StaticEntity*> obstuctors, Camera* cam);

	const std::vector<glm::vec2> &GetSightTriangles() const;

private:

	static glm::mat2 sRotLeft;
	static glm::mat2 sRotRight;

	std::vector<Ray> sightVertices;
	std::vector<glm::vec2> sightTriangles;

	void AddCheckVertex(glm::vec2 v, std::vector<Ray> &rays);
	void AddHitPoly(HitPoly &hP, Entity* e, glm::vec2 &pos, std::vector<Line> &lines, std::vector<Ray> &rays, HitBox &bounds);

	GLboolean CheckForBounds(const HitBox &bounds, const glm::vec2 &v, const GLfloat &range);
	GLboolean CheckForBounds(const HitBox &bounds, const glm::vec2 &v);

	void AddScreenClip(const HitBox &bounds, const glm::vec2 &v1, const glm::vec2 &v2, std::vector<Ray> &rays);
};
