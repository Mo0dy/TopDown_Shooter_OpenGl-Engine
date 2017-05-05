#include "Renderer.h"
#include "ResourceManager.h"
#include "Game.h"

myVertex::myVertex(glm::vec2 position): pos(position), color(glm::vec3(0.0f, 0.0f, 0.0f))
{
}
myVertex::myVertex(glm::vec2 position, glm::vec3 inColor): pos(position), color(inColor)
{
}

std::vector<myVertex> Renderer::drawLineBuffer;

Renderer::Renderer(std::string shader) :myShader(shader)
{
	initRenderData();
}

void Renderer::initRenderData() {
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
}

void Renderer::RenderSprite(Entity &entity, Camera &cam) {
	// Calculating transformation matrix
	glm::mat4 model;
	// Transform to Camera
	model = glm::ortho(0.0f, cam.size.x, 0.0f, cam.size.y, -1.0f, 1.0f);
	model = glm::translate(model, glm::vec3(-cam.pos, 0.0f));
	// Transform to WCO
	model = glm::translate(model, glm::vec3(entity.pos, 0.0f));
	model = glm::rotate(model, entity.angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * entity.size.x, -0.5f * entity.size.y, 0.0f));
	model = glm::scale(model, glm::vec3(entity.size, 1.0f));

	ResourceManager::GetShader(myShader).SetMatrix4("model", model); // Maybe we should store the shader in the render object?
	ResourceManager::GetShader(myShader).SetVector3f("spriteColor", entity.color);

	// Rendering
	ResourceManager::GetShader(myShader).Use();
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture(entity.tex).Bind();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6); // This needs to be modified when we start using vectors to store vertex data in entities
	glBindVertexArray(0);
}

void Renderer::RenderBuffer(Camera &cam) {
	if (drawLineBuffer.size() > 0) {
		GLuint VBO, VAO;

		std::vector<GLfloat> vertices;
		for (myVertex v : drawLineBuffer) {
			vertices.push_back(v.pos.x);
			vertices.push_back(v.pos.y);
			vertices.push_back(v.color.x);
			vertices.push_back(v.color.y);
			vertices.push_back(v.color.z);
		}
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size(), &vertices[0], GL_STATIC_DRAW);
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
		glLineWidth(5);
		glDrawArrays(GL_LINES, 0, drawLineBuffer.size());
		glBindVertexArray(0);
		drawLineBuffer.clear();
	}
}