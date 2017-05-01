#pragma once
#include "Entity.h"
#include "Shader.h"

class Renderer
{
public:
	Renderer(std::string shader);
	void DrawEntity(Entity &entity);
private:
	std::string myShader;
};

