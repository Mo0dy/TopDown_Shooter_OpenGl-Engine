#include "LevelTest.h"
#include "E_Drone.h"
#include "E_MotherDrone.h"
#include "StaticEntity.h"
#include "E_Jelly.h"

void LevelTest::loadLevelTest() {
	ResourceManager::LoadEtex("Textures", "Island", ".jpg", GL_TRUE, "LevelTest_T_Island", GL_FALSE);
}

LevelTest::LevelTest()
{
	background = Entity(glm::vec2(0), ResourceManager::GetEtex("LevelTest_T_Island"));
	background.etex.setTexSize(150);
	size = background.etex.getTexSize();
	//entities.push_back(new StaticEntity(glm::vec2(10, 10), 10, 0.25f * glm::pi<GLfloat>(), testEtex, GL_TRUE));
	//entities.back()->Hitboxes = testEtex.getAbsHitboxes();
}

LevelTest::~LevelTest()
{
}

GLfloat time;

void LevelTest::updateL(GLfloat dt) {


	//if (Game::Enemies.size() < Game::Players.size() * 2) {
	//	if (glm::length(Game::Enemies[0]->getVel()) > 0) {
	//		Game::Enemies.push_back(new E_Drone(Game::Enemies[0]->pos + glm::normalize(Game::Enemies[0]->getVel()) * -0.5f * Game::Enemies[0]->etex.getTexSize().y));
	//	}
	//}

	// Adding child drones
	for (int i = 0; i < Game::Enemies.size(); i++) {
		E_MotherDrone* testMDrone = dynamic_cast<E_MotherDrone*>(Game::Enemies[i]);
		if (testMDrone != NULL) {
			if (!testMDrone->death) {
				if (Game::Enemies.size() < Game::Players.size() * 10) {
					if (glm::length(Game::Enemies[0]->getVel()) > 0) {
						Game::Enemies.push_back(new E_Drone(testMDrone->pos + glm::normalize(testMDrone->getVel()) * -0.5f * testMDrone->etex.getTexSize().y));
					}
				}
			}
		}
	}

	// Multiplying jelly
	for (int i = 0; i < Game::Enemies.size(); i++) {
		E_Jelly* testJelly = dynamic_cast<E_Jelly*>(Game::Enemies[i]);
		if (testJelly != NULL) {
			if (testJelly->death) {
				if (testJelly->jellySize > 0.4)
				{
					Game::Enemies.push_back(new E_Jelly(testJelly->pos + glm::vec2(1), testJelly->jellySize / glm::sqrt(2)));
					Game::Enemies.push_back(new E_Jelly(testJelly->pos + glm::vec2(1.5), testJelly->jellySize / glm::sqrt(2)));
				}
			}
		}
	}

	if (Game::Enemies.size() == 0) {
		if (wavecounter < MAX_WAVE) {
			spawnNextWave();
		}
		else {
			std::cout << "================  VICTORY  ==================" << std::endl;
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
	wavecounter = 0;

	spawnNextWave();

	for (int i = 0; i < Util::PLAYER_AMOUNT; i++) {
		Game::Players.push_back(new Robot(glm::vec2(0, 3 * i)));
		Game::Players.back()->color = Util::PLAYER_COLORS[i];
	}
		
	//glm::vec2 motherDrone_SpawnPos;

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
	//Game::Enemies.push_back(new E_Jelly(glm::vec2(5), 1));

	//for (int i = 0; i < 1; i++) {
	//	for (int j = 0; j < 1; j++) {
	//		Game::Enemies.push_back(new E_Drone(glm::vec2(3 + i * 3, 3 + j * 3)));
	//	}
	//}
}

void LevelTest::spawnNextWave() {

	std::cout << "WAVE[" << wavecounter + 1 << "]" << std::endl;

	glm::vec2 wave_SpawnPos;

	switch (rand() % 4) {
	case 0: wave_SpawnPos = glm::vec2(0, -50);
		break;
	case 1: wave_SpawnPos = glm::vec2(0, 50);
		break;
	case 2: wave_SpawnPos = glm::vec2(-50, 0);
		break;
	case 3: wave_SpawnPos = glm::vec2(50, 0);
		break;
	}

	for (int i = 0; i < WAVES[wavecounter][DRONE]; i++) {
		Game::Enemies.push_back(new E_Drone(wave_SpawnPos));
	}
	for (int i = 0; i < WAVES[wavecounter][MOTHER_DRONE]; i++) {
		Game::Enemies.push_back(new E_MotherDrone(wave_SpawnPos));
	}
	for (int i = 0; i < WAVES[wavecounter][SLIME]; i++) {
		Game::Enemies.push_back(new E_Jelly(wave_SpawnPos, 1));
	}
	wavecounter++;
}