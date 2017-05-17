#include "Level.h"
#include "Game.h"

Level::Level()
{
}

Level::~Level()
{
	for (Entity* e : entities) {
		delete e;
	}
}

void Level::UpdateL(GLfloat dt) {
}

void Level::Reset() {
}

