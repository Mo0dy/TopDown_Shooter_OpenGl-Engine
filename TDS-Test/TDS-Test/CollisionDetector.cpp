#include "CollisionDetector.h"
#include "Renderer.h"

CollisionDetector::CollisionDetector()
{
}

CollisionDetector::~CollisionDetector()
{
}

GLboolean CollisionDetector::DoCCheck(Entity* e1, Entity* e2, GLfloat* const penDepth, glm::vec2* const minColAxis)
{
	// Rough check for possible texture size intersection
	if (glm::distance(e1->Get2DPos(), e2->Get2DPos()) > (glm::length(e1->Get2DSize()) + glm::length(e2->Get2DSize())) * 0.5f) { return GL_FALSE; }
	
	// Checks all hitObjects for each other.
	for (HitPoly hP1 : e1->GetHitComb().hitBoxes)
	{
		for (HitPoly hP2 : e2->GetHitComb().hitBoxes)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitPoly hP2 : e2->GetHitComb().hitPolys)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitCircle hC2 : e2->GetHitComb().hitCircles)
		{
			if (DoCPCheck(hC2, hP1, penDepth, minColAxis)) 
			{ 
				*minColAxis *= -1; // because hC2 is from the second entity the direction of the axes has to be reversed
				return GL_TRUE; 
			}
		}
	}
	for (HitPoly hP1 : e1->GetHitComb().hitPolys)
	{
		for (HitPoly hP2 : e2->GetHitComb().hitBoxes)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitPoly hP2 : e2->GetHitComb().hitPolys)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitCircle hC2 : e2->GetHitComb().hitCircles)
		{
			if (DoCPCheck(hC2, hP1, penDepth, minColAxis))
			{
				*minColAxis *= -1; // because hC2 is from the second entity the direction of the axes has to be reversed
				return GL_TRUE;
			}
		}
	}
	for (HitCircle hC1 : e1->GetHitComb().hitCircles)
	{
		for (HitPoly hP2 : e2->GetHitComb().hitBoxes)
		{
			if (DoCPCheck(hC1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitPoly hP2 : e2->GetHitComb().hitPolys)
		{
			if (DoCPCheck(hC1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitCircle hC2 : e2->GetHitComb().hitCircles)
		{
			if (DoCCCheck(hC1, hC2, penDepth, minColAxis)) { return GL_TRUE; }
		}
	}
	
	return GL_FALSE; // No Collision detected
}

GLboolean CollisionDetector::DoPPCheck(HitPoly& hP1, HitPoly& hP2, GLfloat* colDepth, glm::vec2* const minColAxis)
{
	if (glm::distance(hP1.GetPos(), hP2.GetPos()) > hP1.GetMaxDist() + hP2.GetMaxDist()) { return GL_FALSE; } // Rough check

	// it doasnt matter weather I translate both or only one (relatively to the other) we should probably do that later on to optimize speed
	HitPoly tAbsHP1 = hP1;
	tAbsHP1.Translate(hP1.GetPos()); // every vertex is now relative to the WCO
	HitPoly tAbsHP2 = hP2;
	tAbsHP2.Translate(hP2.GetPos()); // every vertex is now relative to the WCO

	// collects all axes that have to be checked for
	std::vector<glm::vec2> axes;
	axes.reserve(tAbsHP1.GetAxes().size() + tAbsHP2.GetAxes().size());
	axes.insert(axes.end(), tAbsHP1.GetAxes().begin(), tAbsHP1.GetAxes().end());
	axes.insert(axes.end(), tAbsHP2.GetAxes().begin(), tAbsHP2.GetAxes().end());

	// Checks for interval intersection on every axis. If only one has none there is no collision.
	GLfloat* E1dist; // these hold the minimum [0] and maximum [1] interval borders of both Polygons
	GLfloat* E2dist;

	GLfloat iDepth;
	GLfloat minIDepth = 100;

	GLfloat axisDir; // This is either -1 or 1 making the returned axis always point from the first hitbox to the second
	for (glm::vec2 axis : axes)
	{
		E1dist = tAbsHP1.GetMinMaxProj(axis);
		E2dist = tAbsHP2.GetMinMaxProj(axis);

		// Checks for interval intersection
		if (E1dist[1] > E2dist[1]) {
			if (E1dist[0] > E2dist[1]) { return GL_FALSE; }
		} else if (E1dist[1] < E2dist[0]) { return GL_FALSE; }

		// gets the amount of interval overlapping
		if (E1dist[0] > E2dist[0]) {
			iDepth = E2dist[1] - E1dist[0];
			axisDir = -1;
		} else {
			iDepth = E1dist[1] - E2dist[0];
			axisDir = 1;
		}
		if (iDepth < minIDepth) {
			*minColAxis = axis *  axisDir;
			minIDepth = iDepth;
		}
	}
	return GL_TRUE;
}

// Code is missing
GLboolean CollisionDetector::DoCPCheck(HitCircle& hC, HitPoly& hP, GLfloat* colDepth, glm::vec2* const minColAxis)
{
	return GL_FALSE;
}

GLboolean CollisionDetector::DoCCCheck(HitCircle& hC1, HitCircle& hC2, GLfloat* colDepth, glm::vec2* const minColAxis)
{
	*colDepth = hC1.GetRadius() + hC2.GetRadius() - glm::distance(hC1.GetPos(), hC2.GetPos()); // Overlap is negativ if there is no collision
	if (*colDepth > 0)
	{
		*minColAxis = hC2.GetPos() - hC1.GetPos();
		return GL_TRUE;
	}
	return GL_FALSE;
}