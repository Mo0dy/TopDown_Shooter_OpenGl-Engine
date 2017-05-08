#include "LevelTest.h"
#include "AwesomeFace.h"

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

void LevelTest::reset() {
	Game::deleteEntities();
	Game::clearEntities();
	Game::Players.push_back(new Robot(glm::vec2(1, 1)));
	Game::Players.back()->setColor(glm::vec3(1.0f, 0.7f, 0.7f));

#ifdef SECOND_PLAYER
	Game::Players.push_back(new Robot(glm::vec2(4, 1)));
	Game::Players.back()->setColor(glm::vec3(0.7f, 0.7f, 1.0f));
#endif // SECOND_PLAYER

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			Game::dynEntities.push_back(new AwesomeFace(glm::vec2(3 + i * 3, 3 + j * 3)));
		}
	}
}