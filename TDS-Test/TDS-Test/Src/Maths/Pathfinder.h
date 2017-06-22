#pragma once

#include "..\Tools\Util.h"
#include "stlastar.h"
#include <map>
#include "..\GameContent\Entities\StaticEntity.h"

struct SinglePoint {
	SinglePoint();
	SinglePoint(GLuint index, glm::vec2 pos);

	GLuint index;
	glm::vec2 pos;
};

class MapNode {
	
public:
	MapNode();
	MapNode(glm::vec2 pos);

	float GoalDistanceEstimate(MapNode &nodeGoal);
	bool IsGoal(MapNode &nodeGoal);
	bool GetSuccessors(AStarSearch<MapNode> *astarsearch, MapNode *parent_node);
	float GetCost(MapNode &successor);
	bool IsSameState(MapNode &rhs);

	glm::vec2 GetPos() const;
	std::vector<MapNode*> GetSuccessors();
	void AddSuccessor(MapNode* mapNode);

private:
	glm::vec2 pos;
	std::vector<MapNode*> successors;

};

class PathMap {
public:
	PathMap();
	~PathMap();

	void AddMapNodes(std::vector<MapNode*> mapNodes);
	void AddMapNode(glm::vec2 pos);

	std::vector<MapNode*> nodes;
};

class Pathfinder
{
public:
	static std::map<std::string, PathMap> sPathMaps;
	static void AddMap(std::string name, PathMap &map);

	static PathMap* CreatePathMap(std::vector<StaticEntity*> statEntities, GLfloat size);

	Pathfinder();
	~Pathfinder();

private:
	static GLboolean CheckSinglePoints(std::vector<SinglePoint> &singlePoints, const HitPoly &hP);
	static GLboolean CheckPointList(const std::vector<SinglePoint> &singlePoints, GLuint index);
	static PathMap* CreateConnections(std::vector<HitPoly> &tHitPolys);
	static void CombineHitPolys(HitPoly hP1, HitPoly hP2, std::vector<SinglePoint> sP1, std::vector<SinglePoint> sP2, GLboolean col1, GLboolean col2, HitPoly &result);
	static void MakePolyConvex(HitPoly &poly);
};
