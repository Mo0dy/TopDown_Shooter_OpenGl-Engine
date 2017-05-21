#pragma once

#include "Util.h"

class HitPoly 
{
public:
	HitPoly();
	HitPoly(const HitPoly &hitPoly, GLfloat angle);
	HitPoly(const HitPoly &rHitPoly, glm::vec2 size);
	HitPoly(const HitPoly &rHitPoly, glm::vec2 size, GLfloat angle);
	virtual void Update();
	virtual void Rotate(GLfloat angle);
	void Scale(glm::vec2 scaleV);
	void Translate(glm::vec2 sV);

	std::vector<glm::vec2> GetAxes() const;
	std::vector<glm::vec2> GetVertices() const;
	GLfloat* GetMinMaxProj(glm::vec2 axis) const;
	glm::vec2 GetPos() const;
	GLfloat GetMaxDist() const;

protected:
	glm::vec2 pos; // vertices are describes relative to this point
	std::vector<glm::vec2> axes;
	std::vector<glm::vec2> vertices;

	GLfloat maxDist;

	void AddAxis(glm::vec2 axis);
	GLboolean CheckAxis(glm::vec2 axis);
};

class HitBox : public HitPoly
{
public:
	HitBox(glm::vec2 pos, glm::vec2 size, GLfloat angle);
	HitBox(const HitBox& hitBox, glm::vec2 pos, GLfloat angle);
	HitBox(const HitBox& rHitBox, glm::vec2 size);
	void Update();
	void Rotate(GLfloat angle);

	glm::vec2 GetSize() const;
	GLfloat GetAngle() const;

protected:
	glm::vec2 size; // Size of the rectangle from lower left corner as proportion to the width and height;
	GLfloat angle; // angle relative to the corresponding texture
};

class HitCircle 
{
public:
	HitCircle();
	HitCircle(glm::vec2 pos, GLfloat radius);
	HitCircle(const HitCircle& rHitCircle, glm::vec2 size); // builds a hitcirle from a relative hitcirlce -> the radius is relative to the width of the master entitiy
	GLfloat* GetMinMaxProj(glm::vec2 axis) const;

	glm::vec2 GetPos() const;
	GLfloat GetRadius() const;

protected:
	glm::vec2 pos;
	GLfloat radius;
};

struct HitComb
{
	HitComb();
	HitComb(const HitComb& hitComb, glm::vec2 size);

	std::vector<HitCircle> hitCircles;
	std::vector<HitBox> hitBoxes;
	std::vector<HitPoly> hitPolys;

	void clear();
};

