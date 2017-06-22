#include "Renderer.h"
#include "ResourceManager.h"
#include "..\Game.h"

myVertex::myVertex(glm::vec2 position) : pos(position), color(glm::vec3(0.0f, 0.0f, 0.0f)) {}
myVertex::myVertex(glm::vec2 position, glm::vec3 inColor) : pos(position), color(inColor) {}

std::vector<myVertex> Renderer::sDrawLineBuffer;
std::vector<myVertex> Renderer::sDrawPointBuffer;
std::vector<myVertex> Renderer::sDrawTriangleBuffer;

Renderer::Renderer(std::string shader, std::string hideSightShader, std::string ignoreSightShader, GLuint widht, GLuint height) : myShader(shader), hideSightShader(hideSightShader), ignoreSightShader(ignoreSightShader)
{
	initRenderData(widht, height);
}

void Renderer::initRenderData(GLuint width, GLuint height) {
	// Creating Framebuffer for sight Rendering
	glGenFramebuffers(1, &sightFbo);
	glGenTextures(1, &sightTex);
	glBindFramebuffer(GL_FRAMEBUFFER, sightFbo);
	glBindTexture(GL_TEXTURE_2D, sightTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, sightTex, 0);

	// Configure VAO/VBO
	GLuint VBO;
	GLfloat vertices[] = { // in cm
						   // Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindVertexArray(quadVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Calculating sight matrix

	sight = glm::translate(sight, glm::vec3(-0.5f, -0.5f, 0.0f));
	sight = glm::scale(sight, glm::vec3(0.5f));
}

void Renderer::RenderSprite(Entity &entity, Camera &cam, RENDER_TYPE renderType) {
	// Calculating transformation matrix

	std::string shader;

	switch (renderType) {
	case NORMAL: shader = this->myShader;
		break;
	case HIDE: shader = this->hideSightShader;
		break;
	case IGNORE_SIGHT: shader = this->ignoreSightShader;
		break;
	}

	glm::mat4 model;
	// Transform to Camera
	model = glm::ortho(0.0f, cam.size.x, 0.0f, cam.size.y, -1.0f, 1.0f);
	model = glm::translate(model, glm::vec3(-cam.pos, 0.0f));
	// Transform to WCS
	model = glm::translate(model, glm::vec3(entity.Get2DPos(), 0.0f));
	model = glm::rotate(model, entity.GetAngle(), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * entity.Get2DSize().x, -0.5f * entity.Get2DSize().y, 0.0f));
	model = glm::scale(model, glm::vec3(entity.Get2DSize() * entity.GetZPos(), 1.0f));

	// Calculating sight transformation matrix from 0 - 1 to -1 - 1

	ResourceManager::GetShader(shader).SetMatrix4("model", model); // Maybe we should store the shader in the render object?

	if (renderType != IGNORE_SIGHT) {
		ResourceManager::GetShader(shader).SetMatrix4("sight", sight);
		ResourceManager::GetShader(shader).SetInt("sightMap", 1);
	}
	
	ResourceManager::GetShader(shader).SetVector3f("spriteColor", entity.GetColor());

	// Rendering
	ResourceManager::GetShader(shader).SetInt("image", 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ResourceManager::GetShader(shader).Use();
	glActiveTexture(GL_TEXTURE0);
	entity.GetTex()->Bind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sightTex);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6); // This needs to be modified when we start using vectors to store vertex data in entities
	glBindVertexArray(0);
}

void Renderer::RenderBuffer(Camera &cam) {
	if (sDrawLineBuffer.size() > 0) {
		GLuint VBO, VAO;

		std::vector<GLfloat> vertices;
		for (myVertex v : sDrawLineBuffer) {
			vertices.push_back(v.pos.x);
			vertices.push_back(v.pos.y);
			vertices.push_back(v.color.x);
			vertices.push_back(v.color.y);
			vertices.push_back(v.color.z);
		}
		for (myVertex v : sDrawPointBuffer) {
			vertices.push_back(v.pos.x);
			vertices.push_back(v.pos.y);
			vertices.push_back(v.color.x);
			vertices.push_back(v.color.y);
			vertices.push_back(v.color.z);
		}
		for (myVertex v : sDrawTriangleBuffer) {
			vertices.push_back(v.pos.x);
			vertices.push_back(v.pos.y);
			vertices.push_back(v.color.x);
			vertices.push_back(v.color.y);
			vertices.push_back(v.color.z);
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Calculating transformation matrix
		glm::mat4 model;
		// Transform to Camera
		model = glm::ortho(0.0f, cam.size.x, 0.0f, cam.size.y, -1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-cam.pos, 0.0f));
		ResourceManager::GetShader("quadShader").SetMatrix4("model", model);
		ResourceManager::GetShader("quadShader").Use();
		glDrawArrays(GL_TRIANGLES, sDrawLineBuffer.size() + sDrawPointBuffer.size(), sDrawTriangleBuffer.size());
		glLineWidth(3);
		glDrawArrays(GL_LINES, 0, sDrawLineBuffer.size());


		glPointSize(7);
		glDrawArrays(GL_POINTS, (GLint) sDrawLineBuffer.size(), (GLsizei) sDrawPointBuffer.size());
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
		sDrawLineBuffer.clear();
		sDrawPointBuffer.clear();
		sDrawTriangleBuffer.clear();
	}
}

void Renderer::RenderSightMap(Camera &cam, const std::vector<glm::vec2> &triangles)
{
	if (triangles.size() > 2) {
		GLuint VBO, VAO;

		std::vector<GLfloat> vertices;
		for (glm::vec2 v : triangles) {
			vertices.push_back(v.x);
			vertices.push_back(v.y);
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Calculating transformation matrix
		glm::mat4 model;
		// Transform to Camera
		model = glm::ortho(0.0f, cam.size.x, 0.0f, cam.size.y, -1.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-cam.pos, 0.0f));
		ResourceManager::GetShader("sightShader").SetMatrix4("model", model);
		ResourceManager::GetShader("sightShader").Use();
		glBindFramebuffer(GL_FRAMEBUFFER, sightFbo);
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei) triangles.size());
		glBindVertexArray(0);
	}
}

void Renderer::RenderHud() {
	glm::mat4 model2;
	model2 = glm::translate(model2, glm::vec3(0.5f, -1.0f, 0.0f));
	model2 = glm::scale(model2, glm::vec3(0.5f, 0.5f, 1.0f));

	ResourceManager::GetShader(myShader).SetMatrix4("model", model2); // Maybe we should store the shader in the render object?
	ResourceManager::GetShader(myShader).SetVector3f("spriteColor", glm::vec3(1.0f));

	// Rendering
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ResourceManager::GetShader(myShader).Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sightTex);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6); // This needs to be modified when we start using vectors to store vertex data in entities
	glBindVertexArray(0);
}