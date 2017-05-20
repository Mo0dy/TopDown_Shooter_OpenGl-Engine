#pragma once

#include "Level.h"
#include "Game.h"
#include "LevelTest.h"

class LevelBanana : public Level
{
public:
	GLuint wavecounter;

	static void loadLevelBanana();

	void UpdateL(GLfloat dt);

	LevelBanana();
	~LevelBanana();

	void Reset();

protected:
	void spawnNextWave();
};

