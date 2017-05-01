#include "Renderer.h"
#include "ResourceManager.h"


Renderer::Renderer(std::string shader):myShader(shader)
{
}

void Renderer::DrawEntity(Entity &entity) {
	ResourceManager::GetShader(myShader).Use();

	glActiveTexture(GL_TEXTURE0);
	ResourceManager::GetTexture(entity.getTexture()).Bind();

	glBindVertexArray(entity.getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6); // This needs to be modified when we start using vectors to store vertex data in entities
	glBindVertexArray(0);
}