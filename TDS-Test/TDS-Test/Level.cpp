#include "Level.h"



Level::Level(Background* b): background(b)
{
}


Level::~Level()
{
	delete background;
	for (Entity* e : entities) {
		delete e;
	}
}
