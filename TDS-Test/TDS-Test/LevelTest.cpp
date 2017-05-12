#include "LevelTest.h"
#include "E_Drone.h"
#include "E_MotherDrone.h"
#include "StaticEntity.h"

void LevelTest::loadLevelTest() {
	ResourceManager::LoadEtex("Textures", "Island", ".jpg", GL_TRUE, "LevelTest_T_Island", GL_FALSE);
}

LevelTest::LevelTest()
{
	background = Entity(glm::vec2(0), ResourceManager::GetEtex("LevelTest_T_Island"));
	background.etex.setTexSize(150);
	//entities.push_back(new StaticEntity(glm::vec2(10, 10), 10, 0.25f * glm::pi<GLfloat>(), testEtex, GL_TRUE));
	//entities.back()->Hitboxes = testEtex.getAbsHitboxes();
}

LevelTest::~LevelTest()
{
}

GLfloat time;

void LevelTest::updateL(GLfloat dt) {
	if (!Game::Players[0]->death) {
		time += dt;

		//std::cout << "Time = " << time << std::endl;
	}

	if (Game::Enemies.size() < Game::Players.size() * 10) {
		if (glm::length(Game::Enemies[0]->getVel()) > 0) {
			Game::Enemies.push_back(new E_Drone(Game::Enemies[0]->pos - glm::normalize(Game::Enemies[0]->getVel()) * -0.5f * Game::Enemies[0]->etex.getTexSize().y));
		}
	}

	//while (Game::Enemies.size() < 30) {
	//	switch (rand() % 4) {
	//	case 0: Game::Enemies.push_back(new E_Drone(glm::vec2(0, -50)));
	//		break;
	//	case 1: Game::Enemies.push_back(new E_Drone(glm::vec2(0, 50)));
	//		break;
	//	case 2: Game::Enemies.push_back(new E_Drone(glm::vec2(-50, 0)));
	//		break;
	//	case 3: Game::Enemies.push_back(new E_Drone(glm::vec2(50, 0)));
	//		break;
	//	}
	//}

}

void LevelTest::reset() {
	time = 0;
	Game::deleteEntities();
	Game::clearEntities();


	for (int i = 0; i < PLAYER_AMOUNT; i++) {
		Game::Players.push_back(new Robot(glm::vec2(0, 3 * i)));
		Game::Players.back()->color = PLAYER_COLOR[i];
	}
		
	glm::vec2 motherDrone_SpawnPos;

		switch (rand() % 4) {
		case 0: motherDrone_SpawnPos = glm::vec2(0, -50);
			break;
		case 1: motherDrone_SpawnPos = glm::vec2(0, 50);
			break;
		case 2: motherDrone_SpawnPos = glm::vec2(-50, 0);
			break;
		case 3: motherDrone_SpawnPos = glm::vec2(50, 0);
			break;
		}

	Game::Enemies.push_back(new E_MotherDrone(motherDrone_SpawnPos));

	//for (int i = 0; i < 1; i++) {
	//	for (int j = 0; j < 1; j++) {
	//		Game::Enemies.push_back(new E_Drone(glm::vec2(3 + i * 3, 3 + j * 3)));
	//	}
	//}
}