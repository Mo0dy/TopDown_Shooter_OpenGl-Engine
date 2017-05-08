#include "Level.h"
#include "Game.h"

Level::Level()
{
}

Level::~Level()
{
	delete background;
	for (Entity* e : entities) {
		delete e;
	}
}

void Level::updateL(GLfloat dt) {
}

void Level::reset() {
}

