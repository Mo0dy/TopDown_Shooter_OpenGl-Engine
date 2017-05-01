#pragma once
#include "Entity.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer(const Shader &shader);
	~Renderer();

	void DrawEntity(Entity &entity);
private:
	Shader shader;
};

