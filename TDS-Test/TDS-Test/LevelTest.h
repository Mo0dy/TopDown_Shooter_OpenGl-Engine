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

