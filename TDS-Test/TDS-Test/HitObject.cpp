#include "HitObject.h"

// Hit poly
HitPoly::HitPoly() {}
HitPoly::HitPoly(const HitPoly &hitPoly, GLfloat angle) : vertices(hitPoly.GetVertices()), axes(hitPoly.GetAxes()) { this->Rotate(angle); }
HitPoly::HitPoly(const HitPoly &rHitPoly, glm::vec2 size) : vertices(rHitPoly.GetVertices()), axes(rHitPoly.GetAxes()) { this->Scale(size * 0.5f); }
HitPoly::HitPoly(const HitPoly &rHitPoly, glm::vec2 size, GLfloat angle) : HitPoly(rHitPoly, size) { this->Rotate(angle); }

void HitPoly::Update() 
{
	for (int i = 1; i < vertices.size(); i++) {
		AddAxis(vertices[i] - vertices[i - 1]);
	}
	AddAxis(vertices.front() - vertices.back());
	maxDist = 0;
	GLfloat tVDist;
	for (glm::vec2 v : vertices)
	{
		tVDist = glm::length(v);
		if (tVDist > maxDist) { maxDist = tVDist; }
	}
}
void HitPoly::AddAxis(glm::vec2 axis)
{
	if (CheckAxis(axis)) { axes.push_back(axis); }
}
GLboolean HitPoly::CheckAxis(glm::vec2 axis)
{
	for (glm::vec2 a : axes) { if (a.x / a.y == axis.x / axis.y) { return GL_TRUE; } }
	return GL_FALSE;
}
GLfloat* HitPoly::GetMinMaxProj(glm::vec2 axis) const
{
	GLfloat minMax[2];
	minMax[0] = glm::dot(axis, vertices[0]);
	minMax[1] = minMax[0];
	GLfloat dotResult;

	for (int i = 1; i < vertices.size(); i++)
	{
		dotResult = glm::dot(axis, vertices[i]);
		if (dotResult > minMax[1]) {
			minMax[1] = dotResult;
		}
		else if (dotResult < minMax[0]) {
			minMax[0] = dotResult;
		}
	}
	return minMax;
}
void HitPoly::Rotate(GLfloat angle)
{
	glm::mat2 rotMat = Util::RotationMat2(angle);
	for (glm::vec2 &v : this->vertices) { v = rotMat * v; }
	for (glm::vec2 &a : this->axes) { a = rotMat * a; }
}
void HitPoly::Scale(glm::vec2 scaleV)
{
	for (glm::vec2& v : this->vertices) { v *= scaleV; }
	for (glm::vec2& a : this->axes) { a *= scaleV; }
	maxDist *= glm::length(scaleV);
}
void HitPoly::Translate(glm::vec2 sV) {
	for (glm::vec2 &v : this->vertices) { v += sV; }
}

std::vector<glm::vec2> HitPoly::GetAxes() const { return axes; }
std::vector<glm::vec2> HitPoly::GetVertices() const { return vertices; }
glm::vec2 HitPoly::GetPos() const { return this->pos; }
GLfloat HitPoly::GetMaxDist() const { return this->maxDist; }

// Hitbox
HitBox::HitBox(glm::vec2 pos, glm::vec2 size, GLfloat angle) : size(size), angle(angle) { this->axes.reserve(2); this->vertices.reserve(4); this->pos = pos; this->Update(); }
HitBox::HitBox(const HitBox& hitBox, glm::vec2 pos, GLfloat angle) : angle(hitBox.GetAngle() + angle), size(hitBox.GetSize()) { this->pos = hitBox.GetPos() + pos; this->Update(); }
HitBox::HitBox(const HitBox& rHitBox, glm::vec2 size) : HitPoly(rHitBox) { this->Scale(size * 0.5f); }

void HitBox::Update()
{
	glm::mat2 rotMat = Util::RotationMat2(this->angle);
	glm::vec2 rotVec1 = rotMat * glm::vec2(this->size.x * 0.5f, 0);
	glm::vec2 rotVec2 = rotMat * glm::vec2(0, this->size.y  * 0.5f);
	this->vertices.clear();
	this->vertices.push_back(this->pos + rotVec1 + rotVec2);
	this->vertices.push_back(this->pos + rotVec1 - rotVec2);
	this->vertices.push_back(this->pos - rotVec1 - rotVec2);
	this->vertices.push_back(this->pos - rotVec1 + rotVec2);

	this->axes.clear();
	this->axes.push_back(rotMat * glm::vec2(1, 0));
	this->axes.push_back(rotMat * glm::vec2(0, 1));
}
void HitBox::Rotate(GLfloat angle)
{
	glm::mat2 rotMat = Util::RotationMat2(angle);
	for (glm::vec2 &v : this->vertices) { v = rotMat * v; }
	for (glm::vec2 &a : this->axes) { a = rotMat * a; }
	this->angle += angle;
}

// Getters and Setters
glm::vec2 HitBox::GetSize() const { return this->size; }
GLfloat HitBox::GetAngle() const { return this->angle; }

// Hit circle
HitCircle::HitCircle() {}
HitCircle::HitCircle(glm::vec2 pos, GLfloat radius) : pos(pos), radius(radius) {}
HitCircle::HitCircle(const HitCircle& rHitCircle, glm::vec2 size) : pos(rHitCircle.GetPos() * 0.5f * size), radius(size.x * rHitCircle.GetRadius()) {}

// Getters and Setters
glm::vec2 HitCircle::GetPos() const { return this->pos; }
GLfloat HitCircle::GetRadius() const { return this->radius; }

// Hit comb
HitComb::HitComb() {}
HitComb::HitComb(const HitComb& hitComb, glm::vec2 size)
{
	for (const HitCircle hC : hitComb.hitCircles) { this->hitCircles.push_back(HitCircle(hC, size)); }
	for (const HitPoly hP : hitComb.hitPolys) { this->hitPolys.push_back(HitPoly(hP, size)); }
	for (const HitBox hB : hitComb.hitBoxes) { this->hitBoxes.push_back(HitBox(hB, size)); }
}

void HitComb::clear() {
	hitCircles.clear();
	hitPolys.clear();
	hitBoxes.clear();
}