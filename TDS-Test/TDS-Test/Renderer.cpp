#include "Renderer.h"
#include "ResourceManager.h"


Renderer::Renderer(const Shader &shader):shader(shader)
{
}


Renderer::~Renderer()
{
}

void Renderer::DrawEntity(Entity &entity) {
	shader.Use();

	glActiveTexture(GL_TEXTURE0);
	Texture2D testTex = ResourceManager::GetTexture(entity.myTexture);
	testTex.Bind();

	glBindVertexArray(entity.getVAO());
	glDrawArrays(GL_TRIANGLES, 0, 6); // This needs to be modified when we start using vectors to store vertex data in entities
	glBindVertexArray(0);
}