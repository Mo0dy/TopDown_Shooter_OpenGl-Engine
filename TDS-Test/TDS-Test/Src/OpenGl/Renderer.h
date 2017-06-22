#pragma once
#include "..\GameContent\Entities\Entity.h"
#include "..\Shaders\Shader.h"
#include "Camera.h"

struct myVertex {
	myVertex(glm::vec2 position);
	myVertex(glm::vec2 position, glm::vec3 inColor);
	glm::vec2 pos;
	glm::vec3 color;
	GLfloat size;
};

enum RENDER_TYPE {
	NORMAL,
	HIDE,
	IGNORE_SIGHT
};

class Renderer
{
public:
	static std::vector<myVertex> sDrawLineBuffer;
	static std::vector<myVertex> sDrawPointBuffer;
	static std::vector<myVertex> sDrawTriangleBuffer;

	Renderer(std::string shader, std::string hideSightShader, std::string ignoreSightShader, GLuint width, GLuint height);

	void initRenderData(GLuint width, GLuint height);
	void RenderSprite(Entity &entity, Camera &cam, RENDER_TYPE renderType);
	void RenderBuffer(Camera &cam);
	void RenderSightMap(Camera &cam, const std::vector<glm::vec2> &triangles);
	void RenderHud();

private:
	glm::mat4 sight;

	GLuint sightFbo;
	GLuint sightTex;

	std::string myShader;
	std::string hideSightShader;
	std::string ignoreSightShader;
	GLuint quadVAO;
};

