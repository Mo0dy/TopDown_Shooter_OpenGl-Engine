#pragma once

#include "Level.h"
#include "Game.h"

#define DRONE 0
#define MOTHER_DRONE 1
#define SLIME 2

static const GLuint WAVES[6][3] = {
	{10, 0, 1},
	{10, 0, 5},
	{0, 1, 0},
	{0, 0, 20},
	{15, 1, 0},
	{0, 2, 0}
};

static const GLuint MAX_WAVE = 3;

struct housePos {
	housePos(GLfloat posX, GLfloat posY, GLfloat angleDegrees, GLfloat width): pos(glm::vec2(posX, posY)), angle(glm::radians(angleDegrees)), width(width) {
	}
	glm::vec2 pos;
	GLfloat angle;
	GLfloat width;
};

static const std::vector<housePos> TOWN {
	housePos(10,40,80,15), 
	housePos(20,33,50,5),
	housePos(20,27,180,5),
	housePos(15,20,310,5),
	housePos(8,18,260,5),
	housePos(0,22,210,5),
	housePos(-2,30,180,5),
	housePos(-10,25,120,5),
};

class LevelTest: public Level
{
public:

	static void loadLevelTest();

	void updateL(GLfloat dt);

	LevelTest();
	~LevelTest();

	void reset();

	GLuint wavecounter;

protected:
	void spawnNextWave();
};

