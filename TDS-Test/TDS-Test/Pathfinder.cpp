#include "Pathfinder.h"
#include "Renderer.h"
#include "SightCalc.h"
#include "CollisionDetector.h"

// SinglePoints

SinglePoint::SinglePoint() {}
SinglePoint::SinglePoint(GLuint index, glm::vec2 pos) : index(index), pos(pos) {}

// MapNode ===================================================================
MapNode::MapNode() {}
MapNode::MapNode(glm::vec2 pos) : pos(pos) {}

float MapNode::GoalDistanceEstimate(MapNode &nodeGoal) { return glm::distance(this->pos, nodeGoal.GetPos()); }
bool MapNode::IsGoal(MapNode &nodeGoal) { return &nodeGoal == this; }
bool MapNode::GetSuccessors(AStarSearch<MapNode> *astarsearch, MapNode *parent_node) {
	for (MapNode* tMapNode : parent_node->successors) {
		astarsearch->AddSuccessor(*tMapNode);
	}

	return true;
}
float MapNode::GetCost(MapNode &successor) { return glm::distance(this->pos, successor.GetPos()); }
bool MapNode::IsSameState(MapNode &rhs) {
	return this->pos == rhs.GetPos();
}

glm::vec2 MapNode::GetPos() const { return pos; }
std::vector<MapNode*> MapNode::GetSuccessors() { return successors; }
void MapNode::AddSuccessor(MapNode* mapNode) { this->successors.push_back(mapNode); }

// PathMap =================================================================
PathMap::PathMap() {}
PathMap::~PathMap() {
	for (MapNode *tMapNode : nodes) {
		delete tMapNode;
	}
}

void PathMap::AddMapNodes(std::vector<MapNode*> mapNodes) {
	this->nodes = mapNodes;
}

void PathMap::AddMapNode(glm::vec2 pos) {
	this->nodes.push_back(new MapNode(pos));
}

// Pathfinder ==============================================================
std::map<std::string, PathMap> Pathfinder::sPathMaps;
void Pathfinder::AddMap(std::string name, PathMap &map) { sPathMaps[name] = map; }

PathMap* Pathfinder::CreatePathMap(std::vector<StaticEntity*> statEntities, GLfloat size)
{
	PathMap *myPathMap;

	// calculating margins
	std::vector<HitPoly> tHitPolys; // a Vector of scaled HitPolys
	HitPoly tHitPoly;
	for (StaticEntity *tSE : statEntities) {
		for (HitPoly hP : tSE->GetHitComb().hitBoxes) {
			tHitPoly = hP;
			tHitPoly = tHitPoly.GetMargin(size);
			tHitPoly = tHitPoly.GetAbsPoly(tSE);
			tHitPoly.Update();
			tHitPolys.push_back(HitPoly(tHitPoly));
		}
		for (HitPoly hP : tSE->GetHitComb().hitPolys) {
			tHitPoly = hP;
			tHitPoly = tHitPoly.GetMargin(size);
			tHitPoly = tHitPoly.GetAbsPoly(tSE);
			tHitPoly.Update();
			tHitPolys.push_back(HitPoly(tHitPoly));
		}
	}

#ifdef DEBUG_PATHFINDING
	for (HitPoly hP : tHitPolys) {
		for (int i = 1; i < hP.GetVertices().size(); i++) {
			Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices()[i], glm::vec3(0.3f, 0.6f, 0.9f)));
			Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices()[i - 1], glm::vec3(0.3f, 0.6f, 0.9f)));
		}
		Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices().front(), glm::vec3(0.3f, 0.6f, 0.9f)));
		Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices().back(), glm::vec3(0.3f, 0.6f, 0.9f)));
	}
#endif // DEBUG_PATHFINDING

	// calculating intersections
	HitPoly tHitPoly1;
	HitPoly tHitPoly2;

	BoundingRectangle tBRect1;
	BoundingRectangle tBRect2;

	std::vector<SinglePoint> tSinglePoints1;
	std::vector<SinglePoint> tSinglePoints2;

	GLboolean tCollision1;
	GLboolean tCollision2;

	glm::vec2 tPolyCombCenter;
	std::vector<Ray> tCombinedVertices;

	for (int i = 0; i < tHitPolys.size(); i++) {
		for (int j = i + 1; j < tHitPolys.size(); j++) {
			tHitPoly1 = tHitPolys[i];
			// for rought check
			tBRect1 = tHitPoly1.GetBoundingRectangle();
			tHitPoly2 = tHitPolys[j];
			// Rough Check by bounding rectangles
			tBRect2 = tHitPoly2.GetBoundingRectangle();
			if (tBRect1.TestForIntersection(tBRect2)) {
				// Exact Check by modified seperating axes
				tSinglePoints1.clear();
				tSinglePoints1.reserve(tHitPoly1.GetVertices().size());
				for (int a = 0; a < tHitPoly1.GetVertices().size(); a++) {
					tSinglePoints1.push_back(SinglePoint(a, tHitPoly1.GetVertices()[a]));
				}
				tSinglePoints2.clear();
				tSinglePoints2.reserve(tHitPoly2.GetVertices().size());
				for (int a = 0; a < tHitPoly2.GetVertices().size(); a++) {
					tSinglePoints2.push_back(SinglePoint(a, tHitPoly2.GetVertices()[a]));
				}

				// Checking all SinglePoints of 2 against HitPoly1
				tCollision2 = Pathfinder::CheckSinglePoints(tSinglePoints2, tHitPoly1);
				// Checking all SinglePoints of 1 against HitPoly2
				tCollision1 = Pathfinder::CheckSinglePoints(tSinglePoints1, tHitPoly2);

				if (tCollision1 || tCollision2) {
					// Creating new HitPoly
					CombineHitPolys(tHitPoly1, tHitPoly2, tSinglePoints1, tSinglePoints2, tCollision1, tCollision2, tHitPoly);
					MakePolyConvex(tHitPoly);
					tHitPoly.Update();

					tHitPolys[j] = HitPoly(tHitPoly);
					tHitPolys.erase(tHitPolys.begin() + i);
					i--;
					break;
				}
			}
		}
	}

#ifdef DEBUG_PATHFINDING
	for (HitPoly hP : tHitPolys) {
		for (int i = 1; i < hP.GetVertices().size(); i++) {
			Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices()[i], glm::vec3(0.5f, 0.3f, 0.5f)));
			Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices()[i - 1], glm::vec3(0.5f, 0.3f, 0.5f)));
		}
		Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices().front(), glm::vec3(0.5f, 0.3f, 0.5f)));
		Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices().back(), glm::vec3(0.5f, 0.3f, 0.5f)));
	}
#endif // DEBUG_PATHFINDING

	//myPathMap = CreateConnections(tHitPolys);
	myPathMap = new PathMap;

	//#ifdef DEBUG_PATHFINDING
	//	for (MapNode *n1 : myPathMap->nodes) {
	//		for (MapNode *n2 : n1->GetSuccessors()) {
	//			Renderer::sDrawLineBuffer.push_back(myVertex(n1->GetPos(), glm::vec3(1.0f, 0.3f, 0.5f)));
	//			Renderer::sDrawLineBuffer.push_back(myVertex(n2->GetPos(), glm::vec3(1.0f, 0.3f, 0.5f)));
	//		}
	//	}
	//#endif // DEBUG_PATHFINDING

	return myPathMap;
}

PathMap* Pathfinder::CreateConnections(std::vector<HitPoly> &tHitPolys)
{

	PathMap *newPathMap = new PathMap();

	// Create all the Nodes
	for (HitPoly hP : tHitPolys) {
		for (glm::vec2 v : hP.GetVertices()) {
			newPathMap->AddMapNode(v);
		}
	}

	// Add all connections to neigbournodes
	GLuint vertexCount = 0;
	std::vector<glm::vec2> tVertices;

	for (HitPoly hP : tHitPolys) {
		tVertices = hP.GetVertices();
		newPathMap->nodes[vertexCount]->AddSuccessor(newPathMap->nodes[vertexCount + tVertices.size() - 1]);
		newPathMap->nodes[vertexCount]->AddSuccessor(newPathMap->nodes[vertexCount + 1]);
		vertexCount++;

		for (int i = 1; i < tVertices.size() - 1; i++) {
			newPathMap->nodes[vertexCount]->AddSuccessor(newPathMap->nodes[vertexCount - 1]);
			newPathMap->nodes[vertexCount]->AddSuccessor(newPathMap->nodes[vertexCount + 1]);
			vertexCount++;
		}

		newPathMap->nodes[vertexCount]->AddSuccessor(newPathMap->nodes[vertexCount - 1]);
		newPathMap->nodes[vertexCount]->AddSuccessor(newPathMap->nodes[vertexCount - tVertices.size() + 1]);
		vertexCount++;
	}

	// Add all possible connections to other Nodes;
	glm::vec2 v1, v2;

	std::vector<glm::vec2> tVertices1;
	std::vector<glm::vec2> tVertices2;

	GLuint vertexACount = 0;
	GLuint vertexBCount = 0;

	for (int i = 0; i < tHitPolys.size(); i++) {
		tVertices1 = tHitPolys[i].GetVertices();
		vertexBCount = vertexACount + tVertices1.size();
		for (int j = i + 1; j < tHitPolys.size(); j++) {
			tVertices2 = tHitPolys[j].GetVertices();
			for (int a = 0; a < tVertices1.size(); a++) {
				v1 = tVertices1[a];
				for (int b = 0; b < tVertices2.size(); b++) {
					v2 = tVertices2[b];
					if (v1 != v2) {
						// Do Hitscan to check weather a connection is possible.
						if (!CollisionDetector::HitscanInBetween(v1, v2, tHitPolys)) {
							// If the connection is possible add the connection to both nodes.
							newPathMap->nodes[vertexACount + a]->AddSuccessor(newPathMap->nodes[vertexBCount + b]);
							newPathMap->nodes[vertexBCount + b]->AddSuccessor(newPathMap->nodes[vertexACount + a]);
						}
					}
				}
			}
			vertexBCount += tVertices2.size();
		}
		vertexACount += tVertices1.size();
	}
	return newPathMap;
}

void Pathfinder::CombineHitPolys(HitPoly hP1, HitPoly hP2, std::vector<SinglePoint> sP1, std::vector<SinglePoint> sP2, GLboolean col1, GLboolean col2, HitPoly &result)
{
	SinglePoint tOpeningPoints1[2];
	SinglePoint tOpeningPoints2[2];

	std::vector<glm::vec2> hP1Vertices = hP1.GetVertices();
	std::vector<glm::vec2> hP2Vertices = hP2.GetVertices();

	GLuint tOpeningPointsFound;
	GLint index1;
	GLint index2;

	GLboolean res1;
	GLboolean res2;

	// finds opening points for polygon merging
	if (col1 && !col2) {
		// add the outer points of tSinglePoints1 from tHitPoly1 to tOpeningPoints1
		tOpeningPointsFound = 0;
		index1 = 0;
		index2 = 1;
		while (tOpeningPointsFound < 2) {
			if (index1 == hP1Vertices.size()) { index1 = 0; }
			if (index2 == hP1Vertices.size()) { index2 = 0; }

			res1 = CheckPointList(sP1, index1);
			res2 = CheckPointList(sP1, index2);

			if (res1 && !res2) {
				// Start of opening points
				tOpeningPoints1[0] = SinglePoint(index2, hP1Vertices[index2]);
				tOpeningPointsFound++;
			}
			else if (res2 && !res1) {
				// End of opening points
				tOpeningPoints1[1] = SinglePoint(index1, hP1Vertices[index1]);
				tOpeningPointsFound++;
			}
			index1++;
			index2++;
		}

		// add the vertices of a collision between one openingPoint1 and one SinglePoint1 with a line of tHitPoly2 to tOpeningPoints2
		index1 = 0;
		index2 = 1;
		while (!CollisionDetector::DoSingleHitscanInBetween(tOpeningPoints1[0].pos, sP1.front().pos, hP2Vertices[index1], hP2Vertices[index2])) {
			index1++;
			index2++;
			if (index1 == hP2Vertices.size()) { index1 = 0; }
			if (index2 == hP2Vertices.size()) { index2 = 0; }
		}
		if (index2 > index1) {
			tOpeningPoints2[0] = SinglePoint(index1, hP2Vertices[index1]);
			tOpeningPoints2[1] = SinglePoint(index2, hP2Vertices[index2]);
		}
		else {
			tOpeningPoints2[0] = SinglePoint(index2, hP2Vertices[index2]);
			tOpeningPoints2[1] = SinglePoint(index1, hP2Vertices[index1]);
		}
	}
	else if (col2 && !col1) {
		// add the outer points of tSinglePoints2 from tHitPoly2 to tOpeningPoints2
		tOpeningPointsFound = 0;
		index1 = 0;
		index2 = 1;
		while (tOpeningPointsFound < 2) {
			if (index1 == hP2Vertices.size()) { index1 = 0; }
			if (index2 == hP2Vertices.size()) { index2 = 0; }

			res1 = CheckPointList(sP2, index1);
			res2 = CheckPointList(sP2, index2);

			if (res1 && !res2) {
				// Start of opening points
				tOpeningPoints2[0] = SinglePoint(index2, hP2Vertices[index2]);
				tOpeningPointsFound++;
			}
			else if (res2 && !res1) {
				// End of opening points
				tOpeningPoints2[1] = SinglePoint(index1, hP2Vertices[index1]);
				tOpeningPointsFound++;
			}
			index1++;
			index2++;
		}
		// add the vertices of a collision between one openingPoint2 and one SinglePoint2 with a line of tHitPoly1 to tOpeningPoints1
		index1 = 0;
		index2 = 1;
		while (!CollisionDetector::DoSingleHitscanInBetween(tOpeningPoints2[0].pos, sP2.front().pos, hP1Vertices[index1], hP1Vertices[index2])) {
			index1++;
			index2++;
			if (index1 == hP1Vertices.size()) { index1 = 0; }
			if (index2 == hP1Vertices.size()) { index2 = 0; }
		}
		if (index2 > index1) {
			tOpeningPoints1[0] = SinglePoint(index1, hP1Vertices[index1]);
			tOpeningPoints1[1] = SinglePoint(index2, hP1Vertices[index2]);
		}
		else {
			tOpeningPoints1[0] = SinglePoint(index2, hP1Vertices[index2]);
			tOpeningPoints1[1] = SinglePoint(index1, hP1Vertices[index1]);
		}
	}
	else {
		// add the outer points of tSinglePoints1 from tHitPoly1 to tOpeningPoints1
		tOpeningPointsFound = 0;
		index1 = 0;
		index2 = 1;
		while (tOpeningPointsFound < 2) {
			if (index1 == hP1Vertices.size()) { index1 = 0; }
			if (index2 == hP1Vertices.size()) { index2 = 0; }

			res1 = CheckPointList(sP1, index1);
			res2 = CheckPointList(sP1, index2);

			if (res1 && !res2) {
				// Start of opening points
				tOpeningPoints1[0] = SinglePoint(index2, hP1Vertices[index2]);
				tOpeningPointsFound++;
			}
			else if (res2 && !res1) {
				// End of opening points
				tOpeningPoints1[1] = SinglePoint(index1, hP1Vertices[index1]);
				tOpeningPointsFound++;
			}
			index1++;
			index2++;
		}
		// add the outer points of tSinglePoints2 from tHitPoly2 to tOpeningPoints2
		tOpeningPointsFound = 0;
		index1 = 0;
		index2 = 1;
		while (tOpeningPointsFound < 2) {
			if (index1 == hP2Vertices.size()) { index1 = 0; }
			if (index2 == hP2Vertices.size()) { index2 = 0; }

			res1 = CheckPointList(sP2, index1);
			res2 = CheckPointList(sP2, index2);

			if (res1 && !res2) {
				// Start of opening points
				tOpeningPoints2[0] = SinglePoint(index2, hP2Vertices[index2]);
				tOpeningPointsFound++;
			}
			else if (res2 && !res1) {
				// End of opening points
				tOpeningPoints2[1] = SinglePoint(index1, hP2Vertices[index1]);
				tOpeningPointsFound++;
			}
			index1++;
			index2++;
		}
	}

	// debugs opening points
#ifdef DEBUG_PATHFINDING
	DRAWPOINT(tOpeningPoints1[0].pos, COLORBLUE);
	DRAWPOINT(tOpeningPoints1[1].pos, COLORBLUE);
	DRAWPOINT(tOpeningPoints2[0].pos, COLORBLUE);
	DRAWPOINT(tOpeningPoints2[1].pos, COLORBLUE);
#endif // DEBUG_PATHFINDING

	// combines the polygons according to their opening points
	index1 = tOpeningPoints1[0].index;

	result = HitPoly();

	while (index1 != tOpeningPoints1[1].index) {
		result.AddVertex(hP1Vertices[index1]);
		index1++;
		if (index1 == hP1Vertices.size()) {
			index1 = 0;
		}
	}
	result.AddVertex(hP1Vertices[index1]);

	index1 = tOpeningPoints2[0].index;

	while (index1 != tOpeningPoints2[1].index) {
		result.AddVertex(hP2Vertices[index1]);
		index1++;
		if (index1 == hP2Vertices.size()) {
			index1 = 0;
		}
	}
	result.AddVertex(hP2Vertices[index1]);
}

void Pathfinder::MakePolyConvex(HitPoly &poly)
{
	std::vector<glm::vec2> vertices = poly.GetVertices();
	GLint index0 = vertices.size() - 1;
	GLint index1 = 0;
	GLint index2 = 1;

	glm::vec2 corner0, corner1, corner2, line1, line2;

	GLfloat angle1, angle2;
	GLboolean removeV;

	// counter clockwise angels are getting bigger
	
	// for all corners
	do {
		removeV = GL_FALSE;
		corner0 = vertices[index0];
		corner1 = vertices[index1];
		corner2 = vertices[index2];

		line1 = corner0 - corner1;
		line2 = corner2 - corner0;

		angle1 = Util::CalcAbsAngle(line1);
		angle2 = Util::CalcAbsAngle(line2);

		if (angle2 + glm::pi<GLfloat>() <= angle1) {
			angle2 += glm::pi<GLfloat>() * 2;
		}

		if (angle1 > angle2) {
			removeV = GL_TRUE;
		}

		if (removeV) {
			vertices.erase(vertices.begin() + index1);
			if (index0 > index1) { index0--; }
			index0--;
			index1--;
			index2--;
		}
		else {
			index0++;
			index1++;
			index2++;
		}

		if (index0 == vertices.size()) { index0 = 0; }
		if (index1 == vertices.size()) { index1 = 0; }
		if (index2 == vertices.size()) { index2 = 0; }
		if (index0 < 0) { index0 = vertices.size() - 1; }
		if (index1 < 0) { index1 = vertices.size() - 1; }
		if (index2 < 0) { index2 = vertices.size() - 1; }
	} while (index1 != 0);

	poly = HitPoly();
	for (glm::vec2 v : vertices) { poly.AddVertex(v); }
}

Pathfinder::Pathfinder() {}

Pathfinder::~Pathfinder() {}

GLboolean Pathfinder::CheckSinglePoints(std::vector<SinglePoint> &singlePoints, const HitPoly &hP)
{
	GLfloat tPolyProjMax;
	GLfloat tPolyProjMin;

	GLfloat tDotRes;

	std::vector<glm::vec2> tVertices;
	GLuint tVerticesSize;

	for (glm::vec2 axis : hP.GetAxes()) {
		// calculating max and min projection on axes of HitPoly

		tVertices = hP.GetVertices();
		tVerticesSize = tVertices.size();

		tPolyProjMax = tPolyProjMin = glm::dot(tVertices.front(), axis);
		for (int i = 1; i < tVerticesSize; i++) {
			tDotRes = glm::dot(tVertices[i], axis);
			if (tDotRes > tPolyProjMax) {
				tPolyProjMax = tDotRes;
			}
			else if (tDotRes < tPolyProjMin) {
				tPolyProjMin = tDotRes;
			}
		}

		for (int i = 0; i < singlePoints.size(); i++) {
			tDotRes = glm::dot(singlePoints[i].pos, axis);
			if (tDotRes > tPolyProjMax || tDotRes < tPolyProjMin) {
				singlePoints.erase(singlePoints.begin() + i);
				i--;
			}
		}
	}
	return singlePoints.size() > 0;
}

GLboolean Pathfinder::CheckPointList(const std::vector<SinglePoint> &singlePoints, GLuint index)
{
	for (SinglePoint p : singlePoints) {
		if (p.index == index) { return GL_TRUE; }
	}
	return GL_FALSE;
}