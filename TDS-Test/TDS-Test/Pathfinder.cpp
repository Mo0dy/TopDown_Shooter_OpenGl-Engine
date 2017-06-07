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
				tCollision1 = Pathfinder::CheckSinglePoints(tSinglePoints2, tHitPoly1);
				// Checking all SinglePoints of 1 against HitPoly2
				tCollision2 = Pathfinder::CheckSinglePoints(tSinglePoints1, tHitPoly2);

				if (tCollision1 || tCollision2) {
					// combine the two polygons: tHitPoly1 remains and tHitPoly2 will be removed
					// setting the center for the reordering of the polygons later on.
					if (tCollision1) {
						tPolyCombCenter = tSinglePoints2.front().pos;
					}
					else {
						tPolyCombCenter = tSinglePoints1.front().pos;
					}

					tCombinedVertices.clear();
					tCombinedVertices.reserve(tHitPoly1.GetVertices().size() + tHitPoly2.GetVertices().size() - tSinglePoints1.size() - tSinglePoints2.size());

					for (int i = 0; i < tHitPoly1.GetVertices().size(); i++) {
						if (!Pathfinder::CheckPointList(tSinglePoints1, i)) {
							tCombinedVertices.push_back(tHitPoly1.GetVertices()[i] - tPolyCombCenter);
						}
					}
					for (int i = 0; i < tHitPoly2.GetVertices().size(); i++) {
						if (!Pathfinder::CheckPointList(tSinglePoints2, i)) {
							tCombinedVertices.push_back(tHitPoly2.GetVertices()[i] - tPolyCombCenter);
						}
					}

					// sorting CombinedVertices by Angle
					for (Ray &r : tCombinedVertices) { r.CalcAngle(); }
					Ray::SortForAngle(tCombinedVertices);

					// Creating new HitPoly
					tHitPoly = HitPoly();
					for (Ray r : tCombinedVertices) {
						tHitPoly.AddVertex(r.dir + tPolyCombCenter);
					}

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
			Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices()[i], glm::vec3(0.0f, 0.3f, 0.5f)));
			Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices()[i - 1], glm::vec3(0.0f, 0.3f, 0.5f)));
		}
		Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices().front(), glm::vec3(0.0f, 0.3f, 0.5f)));
		Renderer::sDrawLineBuffer.push_back(myVertex(hP.GetVertices().back(), glm::vec3(0.0f, 0.3f, 0.5f)));
	}
#endif // DEBUG_PATHFINDING

	myPathMap = CreateConnections(tHitPolys);
	//myPathMap = new PathMap;

#ifdef DEBUG_PATHFINDING
	for (MapNode *n1 : myPathMap->nodes) {
		for (MapNode *n2 : n1->GetSuccessors()) {
			Renderer::sDrawLineBuffer.push_back(myVertex(n1->GetPos(), glm::vec3(1.0f, 0.3f, 0.5f)));
			Renderer::sDrawLineBuffer.push_back(myVertex(n2->GetPos(), glm::vec3(1.0f, 0.3f, 0.5f)));
		}
	}
#endif // DEBUG_PATHFINDING

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

void Pathfinder::CombineHitPolys(HitPoly hP1, HitPoly hP2, std::vector<SinglePoint> sP1, std::vector<SinglePoint> sP2, HitPoly &result)
{

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