#include "LevelTest.h"
#include "E_Drone.h"
#include "E_MotherDrone.h"

void LevelTest::loadLevelTest() {
	ResourceManager::LoadTexture("Textures\\Island.jpg", GL_TRUE, "LevelTest_T_Island");
	//ResourceManager::LoadTexture("Textures\\HighResBackground.jpg", GL_TRUE, "LevelTest_T_HighResBackground");
}

LevelTest::LevelTest()
{
	size = glm::vec2(150);
	background = new Background("LevelTest_T_Island", size);
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

	//if (Game::Enemies.size() < Game::Players.size() * 10) {
	//	if (glm::length(Game::Enemies[0]->vel) > 0) {
	//		Game::Enemies.push_back(new E_Drone(Game::Enemies[0]->pos - glm::normalize(Game::Enemies[0]->vel) * 0.2f));
	//	}
	//}

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


	for (int i = 0; i < PLAYER_AMOUNT; i++) {
		Game::Players.push_back(new Robot(glm::vec2(0, 3 * i)));
		Game::Players.back()->color = PLAYER_COLOR[i];
	}
		
	glm::vec2 motherDrone_SpawnPos;

	//	switch (rand() % 4) {
	//	case 0: motherDrone_SpawnPos = glm::vec2(0, -50);
	//		break;
	//	case 1: motherDrone_SpawnPos = glm::vec2(0, 50);
	//		break;
	//	case 2: motherDrone_SpawnPos = glm::vec2(-50, 0);
	//		break;
	//	case 3: motherDrone_SpawnPos = glm::vec2(50, 0);
	//		break;
	//	}

	//Game::Enemies.push_back(new E_MotherDrone(motherDrone_SpawnPos));

	//for (int i = 0; i < 1; i++) {
	//	for (int j = 0; j < 1; j++) {
	//		Game::Enemies.push_back(new E_Drone(glm::vec2(3 + i * 3, 3 + j * 3)));
	//	}
	//}
}