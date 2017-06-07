#include "HitObject.h"
#include "Entity.h"

// Bounding Rectangle
GLboolean BoundingRectangle::TestForIntersection(const BoundingRectangle &bRect)
{
	if (this->maxCorner.x > bRect.maxCorner.x && this->minCorner.x > bRect.maxCorner.x) { return GL_FALSE; }
	if (this->maxCorner.x < bRect.maxCorner.x && this->maxCorner.x < bRect.minCorner.x) { return GL_FALSE; }
	if (this->maxCorner.y > bRect.maxCorner.y && this->minCorner.y > bRect.maxCorner.y) { return GL_FALSE; }
	if (this->maxCorner.y < bRect.maxCorner.y && this->maxCorner.y < bRect.minCorner.y) { return GL_FALSE; }
	return GL_TRUE;
}

// Hit poly
HitPoly::HitPoly() : angle(0), pos(glm::vec2(0)) {}
HitPoly::HitPoly(const HitPoly &hitPoly) : angle(hitPoly.angle), pos(hitPoly.pos), vertices(hitPoly.vertices), axes(hitPoly.axes), maxDist(hitPoly.maxDist) {}
HitPoly::HitPoly(const HitPoly &hitPoly, GLfloat angle) : angle(0), pos(glm::vec2(0)), vertices(hitPoly.vertices), axes(hitPoly.axes) { this->Rotate(angle); }
HitPoly::HitPoly(const HitPoly &rHitPoly, glm::vec2 size) : angle(0), pos(glm::vec2(0)), vertices(rHitPoly.vertices), axes(rHitPoly.axes), maxDist(rHitPoly.maxDist) {
	this->Scale(size);
	this->Update();
}
HitPoly::HitPoly(const HitPoly &rHitPoly, glm::vec2 size, GLfloat angle) : HitPoly(rHitPoly, size) { this->Rotate(angle); }

void HitPoly::Update()
{
	glm::vec2 vCon;
	axes.clear();
	for (int i = 1; i < vertices.size(); i++) {
		vCon = vertices[i] - vertices[i - 1];
		AddAxis(glm::vec2(-vCon.y, vCon.x));
	}
	vCon = vertices.front() - vertices.back();
	AddAxis(glm::vec2(-vCon.y, vCon.x));
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
	for (glm::vec2 a : axes) { if (a.x / a.y == axis.x / axis.y) { return GL_FALSE; } }
	return GL_TRUE;
}
GLfloat* HitPoly::GetMinMaxProj(glm::vec2 axis) const
{
	GLfloat *minMax = new GLfloat[2];
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
	for (glm::vec2& v : this->vertices) { v *= scaleV * 0.5f; }
	for (glm::vec2& a : this->axes) { a *= scaleV * 0.5f; }
	maxDist *= glm::length(scaleV);
}
void HitPoly::Translate(glm::vec2 sV) {
	for (glm::vec2 &v : this->vertices) { v += sV; }
}

HitPoly HitPoly::GetMargin(GLfloat m) const
{
	HitPoly *margin = new HitPoly;

	for (int i = 2; i < this->vertices.size(); i++) {
		CalcMarginPoints(*margin, vertices[i - 2], vertices[i - 1], vertices[i], m);
	}
	CalcMarginPoints(*margin, vertices[vertices.size() - 2], vertices[vertices.size() - 1], vertices.front(), m);
	CalcMarginPoints(*margin, vertices[vertices.size() - 1], vertices.front(), vertices[1], m);

	return *margin;
}

void HitPoly::CalcMarginPoints(HitPoly &margin, glm::vec2 a, glm::vec2 b, glm::vec2 c, GLfloat m) const
{
	// the vectors building the corner
	glm::vec2 e = b - a;
	glm::vec2 f = c - b;

	// the outward normal vectors of the edges with length m
	glm::vec2 j(-e.y, e.x);
	j = j * m / glm::length(j);
	glm::vec2 k(-f.y, f.x);
	k = k * m / glm::length(k);

	// the offset points c and a
	glm::vec2 cPrime = c + k;
	glm::vec2 aPrime = a + j;

	// the angle seperating vector with length m
	glm::vec2 w = j + k;
	w = w * m / glm::length(w);

	// G is the point on w with dist m to b
	glm::vec2 G = b + w;

	// g is orthogonal to w
	glm::vec2 g(-w.y, w.x);

	// adding the intersections of the lines: aPrime + Re, G + Rg and cPrime + Rf to the margin polygon
	glm::vec2 tResult;
	Util::CalcIntersection(aPrime, e, G, g, tResult);
	margin.AddVertex(tResult);
	Util::CalcIntersection(cPrime, f, G, g, tResult);
	margin.AddVertex(tResult);
}

BoundingRectangle HitPoly::GetBoundingRectangle() const
{
	BoundingRectangle *bRect = new BoundingRectangle;
	bRect->maxCorner = bRect->minCorner = vertices.front();

	for (int i = 1; i < vertices.size(); i++) {
		if (vertices[i].x > bRect->maxCorner.x) {
			bRect->maxCorner.x = vertices[i].x;
		}
		else if (vertices[i].x < bRect->minCorner.x) {
			bRect->minCorner.x = vertices[i].x;
		}
		if (vertices[i].y > bRect->maxCorner.y) {
			bRect->maxCorner.y = vertices[i].y;
		}
		else if (vertices[i].y < bRect->minCorner.y) {
			bRect->minCorner.y = vertices[i].y;
		}
	}
	return *bRect;
}

std::vector<glm::vec2> HitPoly::GetAxes() const { return axes; }
std::vector<glm::vec2> HitPoly::GetVertices() const { return vertices; }
glm::vec2 HitPoly::GetPos() const { return this->pos; }
GLfloat HitPoly::GetMaxDist() const { return this->maxDist; }
GLfloat HitPoly::GetAngle() const { return this->angle; }
void HitPoly::SetPos(glm::vec2 pos) { this->pos = pos; }
void HitPoly::SetAngle(GLfloat angle) { this->angle = angle; }
void HitPoly::AddVertex(glm::vec2 v) { this->vertices.push_back(v); }

HitPoly HitPoly::GetAbsPoly(Entity* e) const
{
	HitPoly* myHitP = new HitPoly;
	myHitP->vertices = this->vertices;
	myHitP->Rotate(this->angle + e->GetAngle());
	myHitP->Translate(this->pos + e->Get2DPos());
	return *myHitP;
}

HitPoly HitPoly::GetAbsPoly() const
{
	HitPoly* myHitP = new HitPoly;
	myHitP->vertices = this->vertices;
	myHitP->Rotate(this->angle);
	myHitP->Translate(this->pos);
	return *myHitP;
}

// Hitbox
HitBox::HitBox(glm::vec2 pos, glm::vec2 size, GLfloat angle) : size(size) { this->angle = angle;  this->axes.reserve(2); this->vertices.reserve(4); this->pos = pos; }
HitBox::HitBox(const HitBox& hitBox, glm::vec2 pos, GLfloat angle) : size(hitBox.GetSize()) { this->angle = hitBox.GetAngle() + angle; this->pos = hitBox.GetPos() + pos; this->Update(); }
HitBox::HitBox(const HitBox& rHitBox, glm::vec2 size) { this->angle = rHitBox.GetAngle(); this->size = rHitBox.GetSize() * size; this->pos = rHitBox.GetPos() * 0.5f * size; this->Update(); }

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

	maxDist = glm::length(this->pos + this->size * 0.5f);
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

// Hit circle
HitCircle::HitCircle() {}
HitCircle::HitCircle(glm::vec2 vertex, GLfloat radius) : vertex(vertex), radius(radius) {}
HitCircle::HitCircle(const HitCircle& rHitCircle, glm::vec2 size) : vertex(rHitCircle.GetVertex() * 0.5f * size), radius(size.x * rHitCircle.GetRadius()) {}

// Getters and Setters
glm::vec2 HitCircle::GetPos() const { return this->pos; }
glm::vec2 HitCircle::GetVertex() const { return this->vertex; }
GLfloat HitCircle::GetRadius() const { return this->radius; }
void HitCircle::SetPos(glm::vec2 pos) { this->pos = pos; }

// Hit comb
HitComb::HitComb() {}
HitComb::HitComb(const HitComb& hitComb, glm::vec2 size)
{
	for (const HitCircle hC : hitComb.hitCircles) { this->hitCircles.push_back(HitCircle(hC, size)); }
	for (const HitPoly hP : hitComb.hitPolys) { this->hitPolys.push_back(HitPoly(hP, size)); }
	for (const HitBox hB : hitComb.hitBoxes) { this->hitBoxes.push_back(HitBox(hB, size)); }
}

void HitComb::Clear()
{
	hitCircles.clear();
	hitPolys.clear();
	hitBoxes.clear();
}
void HitComb::SetPos(glm::vec2 transVec)
{
	for (HitPoly& hP : this->hitPolys) { hP.SetPos(transVec); }
	for (HitBox& hP : this->hitBoxes) { hP.SetPos(transVec); }
	for (HitCircle& hP : this->hitCircles) { hP.SetPos(transVec); }
}
void HitComb::SetAngle(GLfloat angle) {
	for (HitPoly& hP : this->hitPolys) { hP.SetAngle(angle); }
	for (HitBox& hP : this->hitBoxes) { hP.SetAngle(angle); }
}
