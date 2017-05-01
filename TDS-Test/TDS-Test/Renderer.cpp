#include "Renderer.h"
#include "ResourceManager.h"
#include "Game.h"

Renderer::Renderer(std::string shader):myShader(shader)
{
}

void Renderer::DrawEntity(Entity &entity, Camera &cam) {
	// Calculating transformation matrix
	glm::mat4 model;

	// Transform to Camera
	model = glm::translate(model, glm::vec3(cam.getPos().x, cam.getPos().y, 0.0f));
	model = glm::scale(model, glm::vec3(1 / cam.getPos().z, 1 / cam.getPos().z, 1.0f));

	// Transform to WCO
	model = glm::translate(model, glm::vec3(entity.pos, 0.0f));
	model = glm::rotate(model, entity.angle, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(ENTITY_SCALE, ENTITY_SCALE, 1.0f));

	ResourceManager::GetShader(myShader).SetMatrix4("model", model); // Maybe we should store the shader in the render object?

	// Rendering
	ResourceManager::GetShader(myShader).Use();
	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture(entity.getTexture()).Bind();
	glBindVertexArray(entity.getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6); // This needs to be modified when we start using vectors to store vertex data in entities
	glBindVertexArray(0);
}