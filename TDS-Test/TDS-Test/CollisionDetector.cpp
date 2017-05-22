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
	
	HitComb e1HitComb = e1->GetHitComb();
	HitComb e2HitComb = e2->GetHitComb();
	e1HitComb.SetPos(e1->Get2DPos());
	e2HitComb.SetPos(e2->Get2DPos());
	e1HitComb.SetAngle(e1->GetAngle());
	e2HitComb.SetAngle(e2->GetAngle());

	// Checks all hitObjects for each other.
	for (HitPoly hP1 : e1HitComb.hitBoxes)
	{
		for (HitPoly hP2 : e2HitComb.hitBoxes)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitPoly hP2 : e2HitComb.hitPolys)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitCircle hC2 : e2HitComb.hitCircles)
		{
			if (DoCPCheck(hC2, hP1, penDepth, minColAxis)) 
			{ 
				*minColAxis *= -1; // because hC2 is from the second entity the direction of the axes has to be reversed
				return GL_TRUE; 
			}
		}
	}
	for (HitPoly hP1 : e1HitComb.hitPolys)
	{
		for (HitPoly hP2 : e2HitComb.hitBoxes)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitPoly hP2 : e2HitComb.hitPolys)
		{
			if (DoPPCheck(hP1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitCircle hC2 : e2HitComb.hitCircles)
		{
			if (DoCPCheck(hC2, hP1, penDepth, minColAxis))
			{
				*minColAxis *= -1; // because hC2 is from the second entity the direction of the axes has to be reversed
				return GL_TRUE;
			}
		}
	}
	for (HitCircle hC1 : e1HitComb.hitCircles)
	{
		for (HitPoly hP2 : e2HitComb.hitBoxes)
		{
			if (DoCPCheck(hC1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitPoly hP2 : e2HitComb.hitPolys)
		{
			if (DoCPCheck(hC1, hP2, penDepth, minColAxis)) { return GL_TRUE; }
		}
		for (HitCircle hC2 : e2HitComb.hitCircles)
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
	HitPoly tAbsHP2 = hP2;

	tAbsHP1.Rotate(tAbsHP1.GetAngle());
	tAbsHP2.Rotate(tAbsHP2.GetAngle());
	tAbsHP1.Translate(tAbsHP1.GetPos()); // every vertex is now relative to the WCO
	tAbsHP2.Translate(tAbsHP2.GetPos()); // every vertex is now relative to the WCO

	// collects all axes that have to be checked for
	std::vector<glm::vec2> axes;
	std::vector<glm::vec2> hP1Axes = tAbsHP1.GetAxes();
	std::vector<glm::vec2> hP2Axes = tAbsHP2.GetAxes();
	axes.reserve(tAbsHP1.GetAxes().size() + tAbsHP2.GetAxes().size());
	axes.insert(axes.end(), hP1Axes.begin(), hP1Axes.end());
	axes.insert(axes.end(), hP2Axes.begin(), hP2Axes.end());

#ifdef DEBUG_HITBOXES
	for (int i = 1; i < tAbsHP1.GetVertices().size(); i++) {
		Renderer::drawLineBuffer.push_back(myVertex(tAbsHP1.GetVertices()[i], glm::vec3(0.0f, 0.0f, 1.0f)));
		Renderer::drawLineBuffer.push_back(myVertex(tAbsHP1.GetVertices()[i - 1], glm::vec3(0.0f, 0.0f, 1.0f)));
	}
	Renderer::drawLineBuffer.push_back(myVertex(tAbsHP1.GetVertices().front(), glm::vec3(0.0f, 0.0f, 1.0f)));
	Renderer::drawLineBuffer.push_back(myVertex(tAbsHP1.GetVertices().back(), glm::vec3(0.0f, 0.0f, 1.0f)));
	for (int i = 1; i < tAbsHP2.GetVertices().size(); i++) {
		Renderer::drawLineBuffer.push_back(myVertex(tAbsHP2.GetVertices()[i], glm::vec3(0.0f, 0.0f, 1.0f)));
		Renderer::drawLineBuffer.push_back(myVertex(tAbsHP2.GetVertices()[i - 1], glm::vec3(0.0f, 0.0f, 1.0f)));
	}
	Renderer::drawLineBuffer.push_back(myVertex(tAbsHP2.GetVertices().front(), glm::vec3(0.0f, 0.0f, 1.0f)));
	Renderer::drawLineBuffer.push_back(myVertex(tAbsHP2.GetVertices().back(), glm::vec3(0.0f, 0.0f, 1.0f)));
#endif // DEBUG_HITBOXES


	// Checks for interval intersection on every axis. If only one has none there is no collision.
	GLfloat* tE1dist; // these hold the minimum [0] and maximum [1] interval borders of both Polygons
	GLfloat* tE2dist;

	GLfloat iDepth;
	GLfloat minIDepth = 100;

	GLfloat axisDir; // This is either -1 or 1 making the returned axis always point from the first hitbox to the second
	for (glm::vec2 axis : axes)
	{
		tE1dist = tAbsHP1.GetMinMaxProj(axis);
		tE2dist = tAbsHP2.GetMinMaxProj(axis);

		GLfloat testFloat1 = tE1dist[0];
		GLfloat testFloat2 = tE1dist[1];
		GLfloat testFloat3 = tE2dist[0];
		GLfloat testFloat4 = tE2dist[1];

		// Checks for interval intersection
		if (tE1dist[1] > tE2dist[1]) {
			if (tE1dist[0] > tE2dist[1]) { return GL_FALSE; }
		} else if (tE1dist[1] < tE2dist[0]) { return GL_FALSE; }

		// gets the amount of interval overlapping
		if (tE1dist[0] > tE2dist[0]) {
			iDepth = tE2dist[1] - tE1dist[0];
			axisDir = -1;
		} else {
			iDepth = tE1dist[1] - tE2dist[0];
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