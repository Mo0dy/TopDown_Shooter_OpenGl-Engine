#include "LevelTest.h"
#include "E_Drone.h"
#include "E_MotherDrone.h"
#include "StaticEntity.h"
#include "E_Jelly.h"

void LevelTest::loadLevelTest() {
	ResourceManager::LoadEtex("Textures", "Island", ".jpg", GL_TRUE, "LevelTest_T_Island", HBOX_LOAD_ONE);
	ResourceManager::LoadEtex("Textures\\Houses\\House1", "", ".png", GL_TRUE, "House1", HBOX_LOAD_ONE);
	ResourceManager::LoadEtex("Textures", "Well", ".png", GL_TRUE, "Well", HBOX_AUTOFIT);
}

LevelTest::LevelTest()
{
	background = Entity(&ResourceManager::GetEtex("LevelTest_T_Island"), 150);
	size = background.GetSize();

	for (housePos hP : TOWN) {
		entities.push_back(new StaticEntity(hP.pos, hP.width, hP.angle, ResourceManager::GetEtex("House1"), GL_TRUE));
	}
	entities.push_back(new StaticEntity(glm::vec2(8,27), 3, 0, ResourceManager::GetEtex("Well"), GL_FALSE));
}

LevelTest::~LevelTest()
{
}

GLfloat time;

void LevelTest::UpdateL(GLfloat dt) {


	//if (Game::Enemies.size() < Game::Players.size() * 2) {
	//	if (glm::length(Game::Enemies[0]->getVel()) > 0) {
	//		Game::Enemies.push_back(new E_Drone(Game::Enemies[0]->pos + glm::normalize(Game::Enemies[0]->getVel()) * -0.5f * Game::Enemies[0]->etex.getTexSize().y));
	//	}
	//}

	// Adding child drones
	for (int i = 0; i < Game::sEnemies.size(); i++) {
		E_MotherDrone* testMDrone = dynamic_cast<E_MotherDrone*>(Game::sEnemies[i]);
		if (testMDrone != NULL) {
			if (!testMDrone->GetDeath()) {
				if (Game::sEnemies.size() < Game::sPlayers.size() * 5) {
					if (glm::length(Game::sEnemies[0]->GetVel()) > 0) {
						Game::sEnemies.push_back(new E_Drone(testMDrone->GetPos() + glm::normalize(testMDrone->GetVel()) * -0.5f * testMDrone->GetSize().y));
					}
				}
			}
		}
	}

	// Multiplying jelly
	for (int i = 0; i < Game::sEnemies.size(); i++) {
		E_Jelly* testJelly = dynamic_cast<E_Jelly*>(Game::sEnemies[i]);
		if (testJelly != NULL) {
			if (testJelly->GetDeath()) {
				if (testJelly->jellySize > 0.4)
				{
					Game::sEnemies.push_back(new E_Jelly(testJelly->GetPos() + glm::vec2(1), testJelly->jellySize / glm::sqrt(2)));
					Game::sEnemies.push_back(new E_Jelly(testJelly->GetPos() + glm::vec2(1.5), testJelly->jellySize / glm::sqrt(2)));
				}
			}
		}
	}

	if (Game::sEnemies.size() == 0) {
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

void LevelTest::Reset() {
	//background.etex.setRelHitboxes(ResourceManager::loadrHitboxFromFile("Textures\\HIsland.txt"));
	//background.updateHitboxes();
	//size = background.etex.getTexSize();

	time = 0;
	Game::deleteEntities();
	Game::clearEntities();
	wavecounter = 0;

	spawnNextWave();

	for (int i = 0; i < Util::PLAYER_AMOUNT; i++) {
		Game::sPlayers.push_back(new Robot(glm::vec2(0, 3 * i)));
		Game::sPlayers.back()->SetColor(Util::PLAYER_COLORS[i]);
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
	//Game::Enemies.push_back(new E_Jelly(glm::vec2(5), JELLY_START_SIZE));

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
		Game::sEnemies.push_back(new E_Drone(wave_SpawnPos));
	}
	for (int i = 0; i < WAVES[wavecounter][MOTHER_DRONE]; i++) {
		Game::sEnemies.push_back(new E_MotherDrone(wave_SpawnPos));
	}
	for (int i = 0; i < WAVES[wavecounter][SLIME]; i++) {
		Game::sEnemies.push_back(new E_Jelly(wave_SpawnPos, JELLY_START_SIZE));
	}
	wavecounter++;
}