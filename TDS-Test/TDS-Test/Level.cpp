#include "Level.h"

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
