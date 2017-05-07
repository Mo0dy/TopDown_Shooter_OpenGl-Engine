#include "LevelTest.h"

void LevelTest::loadLevelTest() {
	ResourceManager::LoadTexture("Textures\\Terrain.png", GL_TRUE, "TestTerrain");
}

LevelTest::LevelTest()
{
	size = glm::vec2(100);
	background = new Background ("TestTerrain", size);
}


LevelTest::~LevelTest()
{
}
