#pragma once
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"

struct myVertex {
	myVertex(glm::vec2 position);
	myVertex(glm::vec2 position, glm::vec3 inColor);
	glm::vec2 pos;
	glm::vec3 color;
	GLfloat size;
};

class Renderer
{
public:
	static std::vector<myVertex> sDrawLineBuffer;
	static std::vector<myVertex> sDrawPointBuffer;
	static std::vector<myVertex> sDrawTriangleBuffer;

	Renderer(std::string shader);
	void initRenderData();
	void RenderSprite(Entity &entity, Camera &cam);
	void RenderBuffer(Camera &cam);

private:
	std::string myShader;
	GLuint quadVAO;
};

