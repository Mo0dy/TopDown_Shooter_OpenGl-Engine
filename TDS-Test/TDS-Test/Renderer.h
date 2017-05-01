#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer(std::string shader);
	void DrawEntity(Entity &entity, Camera &cam);
private:
	std::string myShader;
};

