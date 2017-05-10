#include "LevelTest.h"
#include "E_Drone.h"

void LevelTest::loadLevelTest() {
	ResourceManager::LoadTexture("Textures\\Island.jpg", GL_TRUE, "TestTerrain");
}

LevelTest::LevelTest()
{
	size = glm::vec2(150);
	background = new Background ("TestTerrain", size);
}

LevelTest::~LevelTest()
{
}

GLfloat time;

void LevelTest::updateL(GLfloat dt) {
	if (!Game::Players[0]->death) {
		time += dt;

		std::cout << "Time = " << time << " || killcount = " << Game::killcount << std::endl;
	}

	while (Game::Enemies.size() < 30) {
		switch (rand() % 4) {
		case 0: Game::Enemies.push_back(new E_Drone(glm::vec2(0, -50)));
			break;
		case 1: Game::Enemies.push_back(new E_Drone(glm::vec2(0, 50)));
			break;
		case 2: Game::Enemies.push_back(new E_Drone(glm::vec2(-50, 0)));
			break;
		case 3: Game::Enemies.push_back(new E_Drone(glm::vec2(50, 0)));
			break;
		}
	}

}

void LevelTest::reset() {
	Game::killcount = 0;
	time = 0;
	Game::deleteEntities();
	Game::clearEntities();
	Game::Players.push_back(new Robot(glm::vec2(1, 1)));
	Game::Players.back()->setColor(glm::vec3(1.0f, 0.7f, 0.7f));

#ifdef SECOND_PLAYER
	Game::Players.push_back(new Robot(glm::vec2(4, 1)));
	Game::Players.back()->setColor(glm::vec3(0.7f, 0.7f, 1.0f));
#endif // SECOND_PLAYER

	//for (int i = 0; i < 10; i++) {
	//	for (int j = 0; j < 10; j++) {
	//		Game::Enemies.push_back(new E_Drone(glm::vec2(3 + i * 3, 3 + j * 3)));
	//	}
	//}
}