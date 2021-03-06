#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"

struct myVertex {
	myVertex(glm::vec2 position);
	myVertex(glm::vec2 position, glm::vec3 inColor);
	glm::vec2 pos;
	glm::vec3 color;
};

class Renderer
{
public:
	static std::vector<myVertex> drawLineBuffer;

	Renderer(std::string shader);
	void initRenderData();
	void RenderSprite(Entity &entity, Camera &cam);
	void RenderBuffer(Camera &cam);

private:
	std::string myShader;
	GLuint quadVAO;
};

