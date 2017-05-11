#pragma once

#include "Level.h"
#include "Game.h"

class LevelTest: public Level
{
public:

	static void loadLevelTest();

	void updateL(GLfloat dt);

	LevelTest();
	~LevelTest();

	void reset();
};

