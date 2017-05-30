#include "SightCalc.h"
#include "Renderer.h"
#include "CollisionDetector.h"

SightCalc::SightCalc() {}

SightCalc::~SightCalc() {}

void SightCalc::CalcObs(std::vector<Entity*> viewers, std::vector<Entity*> obstuctors, Camera* cam)
{
	sightVertices.clear();

	glm::vec2 tPos;
	glm::vec2 tScanVec;

	glm::mat2 tRotLeft = Util::RotationMat2(0.001f);
	glm::mat2 tRotRight = Util::RotationMat2(glm::pi<GLfloat>() * 2 - 0.002f);

	GLfloat tHitscanRes;

	HitBox tCamHb(cam->pos * 0.5f + cam->size * 0.25f, cam->size, 0.0f);
	tCamHb.Update();
	HitPoly tCamHp = tCamHb.GetAbsPoly();

	Entity* camE = new Entity(glm::vec2(0));
	HitComb tHitComb;
	tHitComb.hitPolys.push_back(tCamHp);

	camE->SetHitComb(tHitComb);

	obstuctors.push_back(camE);

#ifdef DEGUB_SIGHT
	for (int i = 1; i < tCamHp.GetVertices().size(); i++) {
		Renderer::drawLineBuffer.push_back(myVertex(tCamHp.GetVertices()[i - 1], glm::vec3(0.0f, 1.0f, 1.0f)));
		Renderer::drawLineBuffer.push_back(myVertex(tCamHp.GetVertices()[i], glm::vec3(0.0f, 1.0f, 1.0f)));
	}
	Renderer::drawLineBuffer.push_back(myVertex(tCamHp.GetVertices().front(), glm::vec3(0.0f, 1.0f, 1.0f)));
	Renderer::drawLineBuffer.push_back(myVertex(tCamHp.GetVertices().back(), glm::vec3(0.0f, 1.0f, 1.0f)));
#endif // DEBUG_SIGHT

	for (Entity* viewE : viewers) {
		tPos = viewE->Get2DPos(); // the start of the raytrace
		for (Entity* obsE : obstuctors) {
			for (HitPoly hP : obsE->GetHitComb().hitPolys) {
				// this should probably be a function

				HitPoly myHp = hP.GetAbsPoly(obsE);
				for (glm::vec2 vertex : myHp.GetVertices()) {
					if (vertex.x < cam->pos.x + cam->size.x && vertex.x > cam->pos.x && vertex.y < cam->pos.y + cam->size.y && vertex.y > cam->pos.y) {
						tScanVec = vertex - tPos;
						CollisionDetector::DoHitscan(tPos, tScanVec, obstuctors, &tHitscanRes);
						sightVertices.push_back(tScanVec * tHitscanRes);


						tScanVec = tRotLeft * tScanVec;
						CollisionDetector::DoHitscan(tPos, tScanVec, obstuctors, &tHitscanRes);
						sightVertices.push_back( tScanVec * tHitscanRes);


						tScanVec = tRotRight * tScanVec;
						CollisionDetector::DoHitscan(tPos, tScanVec, obstuctors, &tHitscanRes);
						sightVertices.push_back(tScanVec * tHitscanRes);

					}
				}
				}
				// end of the function
			}
		}
	if (sightVertices.size() > 2) {
		sightVertices = SortForAngle(sightVertices);

#ifdef DEBUG_SIGHT
		for (int i = 0; i < sightVertices.size(); i++) {
			Renderer::sDrawLineBuffer.push_back(myVertex(tPos, glm::vec3(0.0f, 1.0f, (GLfloat)i / sightVertices.size())));
			Renderer::sDrawLineBuffer.push_back(myVertex(tPos + sightVertices[i], glm::vec3(0.0f, 1.0f, (GLfloat)i / sightVertices.size())));
			Renderer::sDrawPointBuffer.push_back(myVertex(tPos + sightVertices[i], glm::vec3(1.0f, 0.0f, 0.0f)));
		}
		LOG(sightVertices.size());
#endif // DEBUG_SIGHT

		for (glm::vec2 &v : sightVertices) {
			v += tPos;
		}

#ifdef DEBUG_SIGHT
		for (int i = 1; i < sightVertices.size(); i++) {
			Renderer::sDrawTriangleBuffer.push_back(myVertex(sightVertices[i], glm::vec3(0.0f, 1.0f, 1.0f)));
			Renderer::sDrawTriangleBuffer.push_back(myVertex(sightVertices[i - 1], glm::vec3(0.0f, 1.0f, 1.0f)));
			Renderer::sDrawTriangleBuffer.push_back(myVertex(tPos, glm::vec3(0.0f, 1.0f, 1.0f)));
		}
		Renderer::sDrawTriangleBuffer.push_back(myVertex(sightVertices.back(), glm::vec3(0.0f, 1.0f, 1.0f)));
		Renderer::sDrawTriangleBuffer.push_back(myVertex(sightVertices.front(), glm::vec3(0.0f, 1.0f, 1.0f)));
		Renderer::sDrawTriangleBuffer.push_back(myVertex(tPos, glm::vec3(0.0f, 1.0f, 1.0f)));
		LOG(sightVertices.size());
#endif // DEBUG_SIGHT

	}
}

std::vector<glm::vec2> SightCalc::SortForAngle(std::vector<glm::vec2> vertices)
{
	glm::vec2 tVec;
	for (int i = 1; i < vertices.size(); i++) {
		for (int j = 0; j < vertices.size() - i; j++) {
			if (CompareVectors(vertices[j], vertices[j + 1])) {
				tVec = vertices[j];
				vertices[j] = vertices[j + 1];
				vertices[j + 1] = tVec;
			}
		}
	}
	return vertices;
}

GLboolean SightCalc::CompareVectors(glm::vec2 v1, glm::vec2 v2) { // returns true if v1 has a bigger angle then v2
	return (Util::CalcAbsAngle(v1) > Util::CalcAbsAngle(v2));
}