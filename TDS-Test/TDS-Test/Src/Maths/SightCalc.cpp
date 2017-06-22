#include "SightCalc.h"
#include "..\OpenGl\Renderer.h"
#include "CollisionDetector.h"

#include "..\Tools\Metrics.h"

Line::Line() {}
Line::Line(glm::vec2 v1, glm::vec2 v2) : v1(v1), v2(v2) { this->CalcAngle(); }

void Line::CalcAngle()
{
	this->minAngle = Util::CalcAbsAngle(v1);
	this->maxAngle = Util::CalcAbsAngle(v2);

	if (maxAngle < minAngle) {
		GLfloat tAngle = minAngle;
		minAngle = maxAngle;
		maxAngle = tAngle;
	}
}

void Line::SortForMinAngle(std::vector<Line> &lines)
{
	Line tLine;
	for (int i = 1; i < lines.size(); i++) {
		for (int j = 0; j < lines.size() - i; j++) {
			if (lines[j].minAngle > lines[j + 1].minAngle) {
				tLine = lines[j];
				lines[j] = lines[j + 1];
				lines[j + 1] = tLine;
			}
		}
	}
}

Ray::Ray() {}
Ray::Ray(glm::vec2 dir) : dir(dir) { this->CalcAngle(); }
Ray::Ray(glm::vec2 dir, GLfloat angle) : dir(dir), angle(angle) {}

void Ray::CalcAngle()
{
	this->angle = Util::CalcAbsAngle(dir);
}

void Ray::SortForAngle(std::vector<Ray> &rays)
{
	Ray tRay;
	for (int i = 1; i < rays.size(); i++) {
		for (int j = 0; j < rays.size() - i; j++) {
			if (rays[j].angle > rays[j + 1].angle) {
				tRay = rays[j];
				rays[j] = rays[j + 1];
				rays[j + 1] = tRay;
			}
		}
	}
}

// These matrices rotate the vectors to the vetices slightly right and left so that the ray can pass
glm::mat2 SightCalc::sRotLeft = Util::RotationMat2(0.001f);
glm::mat2 SightCalc::sRotRight = Util::RotationMat2(glm::pi<GLfloat>() * 2 - 0.001f);

SightCalc::SightCalc() {}

SightCalc::~SightCalc() {}

void SightCalc::CalcObs(std::vector<Entity*> viewers, std::vector<StaticEntity*> obstuctors, Camera* cam)
{
	Metrics::GetMetric("SightMetric").LoopBegin();
	sightVertices.clear();

	std::vector<Ray> tRays; // stores all Rays that will be cast
	std::vector<Line> tLines; // stores all Lines that will be hitscanned for

	glm::vec2 tPos; // everything will be tested relative to this position

	tPos = viewers.front()->Get2DPos(); // this is temporary

	Metrics::GetMetric("SightMetric").feelers["Adding"].Start();
	// Adding all Lines and Rays
	// Creates a Hitbox for the camera
	HitBox tCamHb(cam->pos - tPos + cam->size * 0.5f, cam->size, 0.0f);
	tCamHb.Update();

	// The 0th vertex of tCamHb is the upper left one
	// The 2nd vertex of tCamHb is the lower left one

	// Adds the vertices for the camera
	for (int i = 1; i < tCamHb.GetVertices().size(); i++) {
		tLines.push_back(Line(tCamHb.GetVertices()[i], tCamHb.GetVertices()[i - 1]));
		tRays.push_back(tCamHb.GetVertices()[i]);
	}
	tLines.push_back(Line(tCamHb.GetVertices().front(), tCamHb.GetVertices().back()));
	tRays.push_back(tCamHb.GetVertices().front());

	// adding all Lines and Rays for the obstructors
	for (StaticEntity* e : obstuctors)
	{
		if (e->concealing) {
			for (HitPoly hP : e->GetHitComb().hitBoxes) {
				AddHitPoly(hP, e, tPos, tLines, tRays, tCamHb);
			}
			for (HitPoly hP : e->GetHitComb().hitPolys) {
				AddHitPoly(hP, e, tPos, tLines, tRays, tCamHb);
			}
		}
	}
	Metrics::GetMetric("SightMetric").feelers["Adding"].End();

	Metrics::GetMetric("SightMetric").feelers["Sorting"].Start();
	// Sorting Lines and Rays by angle
	Line::SortForMinAngle(tLines);
	Ray::SortForAngle(tRays);
	Metrics::GetMetric("SightMetric").feelers["Sorting"].End();

#ifdef DEBUG_SIGHT
	for (Line l : tLines) {
		Renderer::sDrawLineBuffer.push_back(myVertex(l.v1 + tPos, glm::vec3(0.0f, 1.0f, 0.5f)));
		Renderer::sDrawLineBuffer.push_back(myVertex(l.v2 + tPos, glm::vec3(0.0f, 1.0f, 0.5f)));
	}
	for (Ray r : tRays) {
		Renderer::sDrawLineBuffer.push_back(myVertex(tPos, glm::vec3(1.0f, 0.5f, 0.5f)));
		Renderer::sDrawLineBuffer.push_back(myVertex(r.dir + tPos, glm::vec3(1.0f, 0.5f, 0.5f)));
	}
#endif // DEBUG_SIGHT

	Metrics::GetMetric("SightMetric").feelers["Hitscan"].Start();
	// Doing all the Hitscans in Order
	GLfloat tHitscanRes;
	GLfloat tShortestDist;
	for (Ray r : tRays) {
		tShortestDist = -1;
		for (int i = 0; i < tLines.size(); i++) {
			// if (tLines[i].minAngle > r.angle) { break; } // All remaining Angles will be bigger
			if (tLines[i].maxAngle - tLines[i].minAngle < glm::pi<GLfloat>() && tLines[i].maxAngle < r.angle && tLines[i].minAngle < r.angle) { // this Line will never be intersected again because all remaining Rays have a bigger angle
				tLines.erase(tLines.begin() + i);
				i--;
			}
			else { //if((tLines[i].maxAngle - tLines[i].minAngle < glm::pi<GLfloat>() && tLines[i].maxAngle >= r.angle && tLines[i].minAngle <= r.angle) || (tLines[i].maxAngle - tLines[i].minAngle > glm::pi<GLfloat>() && r.angle < tLines[i].minAngle)){ // actual Hitscan
				tHitscanRes = CollisionDetector::DoSingleHitscan(glm::vec2(0), r.dir, tLines[i].v1, tLines[i].v2);
				if (tHitscanRes >= 0) {
					if (tShortestDist < 0) {
						tShortestDist = tHitscanRes;
					}
					else if (tHitscanRes < tShortestDist) {
						tShortestDist = tHitscanRes;
					}
				}
			}
		}
		if (tShortestDist >= 0) {
			sightVertices.push_back(Ray(tPos + r.dir * tShortestDist, r.angle));
		}
	}
	Metrics::GetMetric("SightMetric").feelers["Hitscan"].End();

#ifdef DEBUG_SIGHT
	for (Ray r : sightVertices) { Renderer::sDrawPointBuffer.push_back(myVertex(r.dir, glm::vec3(1.0f, 0.0f, 0.0f))); }
#endif // DEBUG_SIGHT

	Metrics::GetMetric("SightMetric").feelers["CreatingTriangles"].Start();
	this->sightTriangles.clear();

	if (sightVertices.size() > 1) {
		Ray::SortForAngle(sightVertices);
		for (int i = 1; i < sightVertices.size(); i++) {
			sightTriangles.push_back(sightVertices[i].dir);
			sightTriangles.push_back(sightVertices[i - 1].dir);
			sightTriangles.push_back(tPos);
		}
		sightTriangles.push_back(sightVertices.back().dir);
		sightTriangles.push_back(sightVertices.front().dir);
		sightTriangles.push_back(tPos);
	}
	Metrics::GetMetric("SightMetric").feelers["CreatingTriangles"].End();
	Metrics::GetMetric("SightMetric").LoopEnd();
}

void SightCalc::AddHitPoly(HitPoly &hP, Entity* e, glm::vec2 &pos, std::vector<Line> &lines, std::vector<Ray> &rays, HitBox &bounds) {
	HitPoly tHitPoly;

	// transforms the HitPoly from positions relative to an entity to a position relative to tPos
	if (CheckForBounds(bounds, e->Get2DPos() - pos, hP.GetMaxDist())) { // Rough Check for the whole entity
		tHitPoly = hP.GetAbsPoly(e);
		tHitPoly.Translate(-pos);
		// adds all lines and Rays that are needet
		for (int i = 1; i < tHitPoly.GetVertices().size(); i++) {
			if (CheckForBounds(bounds, tHitPoly.GetVertices()[i])) {
				lines.push_back(Line(tHitPoly.GetVertices()[i], tHitPoly.GetVertices()[i - 1]));
				AddCheckVertex(tHitPoly.GetVertices()[i], rays);

				// Checking for Intersections with the screen
				if (!CheckForBounds(bounds, tHitPoly.GetVertices()[i - 1])) { // one outside one inside of the screen
					AddScreenClip(bounds, tHitPoly.GetVertices()[i], tHitPoly.GetVertices()[i - 1], rays);
				}
			}
			else if (CheckForBounds(bounds, tHitPoly.GetVertices()[i - 1])) {
				lines.push_back(Line(tHitPoly.GetVertices()[i], tHitPoly.GetVertices()[i - 1]));
				// checking for intersections with the screen
				AddScreenClip(bounds, tHitPoly.GetVertices()[i - 1], tHitPoly.GetVertices()[i], rays);
			}
		}
		// the first
		if (CheckForBounds(bounds, tHitPoly.GetVertices().front())) {
			lines.push_back(Line(tHitPoly.GetVertices().front(), tHitPoly.GetVertices().back()));
			AddCheckVertex(tHitPoly.GetVertices().front(), rays);

			// checking for intersections with the screen
			if (!CheckForBounds(bounds, tHitPoly.GetVertices().back())) { // one outside one inside of the screen
				AddScreenClip(bounds, tHitPoly.GetVertices().front(), tHitPoly.GetVertices().back(), rays);
			}
		}
		else if (CheckForBounds(bounds, tHitPoly.GetVertices().back())) {
			lines.push_back(Line(tHitPoly.GetVertices().front(), tHitPoly.GetVertices().back()));
			// checking for intersections with the screen
			AddScreenClip(bounds, tHitPoly.GetVertices().back(), tHitPoly.GetVertices().front(), rays);
		}
	}
}

void SightCalc::AddScreenClip(const HitBox &bounds, const glm::vec2 &v1, const glm::vec2 &v2, std::vector<Ray> &rays) {
	glm::vec2 tRay = v2 - v1;
	GLfloat tHitscanRes;
	GLboolean addedRay = GL_FALSE;

	for (int i = 1; i < bounds.GetVertices().size(); i++) {
		tHitscanRes = CollisionDetector::DoSingleHitscan(v1, tRay, bounds.GetVertices()[i], bounds.GetVertices()[i - 1]);
		if (tHitscanRes >= 0) {
			rays.push_back(v1 + tRay * tHitscanRes);
			addedRay = GL_TRUE;
			break;
		}
	}
	if (!addedRay) {
		tHitscanRes = CollisionDetector::DoSingleHitscan(v1, tRay, bounds.GetVertices().front(), bounds.GetVertices().back());
		if (tHitscanRes >= 0) {
			rays.push_back(v1 + tRay * tHitscanRes);
		}
	}
}

void SightCalc::AddCheckVertex(glm::vec2 v, std::vector<Ray> &rays)
{
	rays.push_back(sRotLeft * v);
	rays.push_back(sRotRight * v);
}

GLboolean SightCalc::CheckForBounds(const HitBox &bounds, const glm::vec2 &v, const GLfloat &range)
{
	if (v.x - range > bounds.GetVertices()[0].x) { return GL_FALSE; }
	if (v.x + range < bounds.GetVertices()[2].x) { return GL_FALSE; }
	if (v.y - range > bounds.GetVertices()[0].y) { return GL_FALSE; }
	if (v.y + range < bounds.GetVertices()[2].y) { return GL_FALSE; }
	return GL_TRUE;
}

GLboolean SightCalc::CheckForBounds(const HitBox &bounds, const glm::vec2 &v)
{
	if (v.x > bounds.GetVertices()[0].x) { return GL_FALSE; }
	if (v.x < bounds.GetVertices()[2].x) { return GL_FALSE; }
	if (v.y > bounds.GetVertices()[0].y) { return GL_FALSE; }
	if (v.y < bounds.GetVertices()[2].y) { return GL_FALSE; }
	return GL_TRUE;
}

// Getters and setters

const std::vector<glm::vec2> &SightCalc::GetSightTriangles() const {
	return sightTriangles;
}