#pragma once

#include "Level.h"
#include "Game.h"
#include "LevelTest.h"

class LevelBanana : public Level
{
public:
	GLuint wavecounter;

	static void loadLevelBanana();

	void updateL(GLfloat dt);

	LevelBanana();
	~LevelBanana();

	void reset();

protected:
	void spawnNextWave();
};

