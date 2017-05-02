#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer(std::string shader);
	void initRenderData();
	void RenderSprite(Entity &entity, Camera &cam);
private:
	std::string myShader;
	GLuint quadVAO;
};

