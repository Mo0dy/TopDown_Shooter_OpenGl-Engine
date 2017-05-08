#pragma once

#include "Level.h"
#include "Game.h"

class LevelTest: public Level
{
public:

	static void loadLevelTest();

	LevelTest();
	~LevelTest();

	void reset();
};

