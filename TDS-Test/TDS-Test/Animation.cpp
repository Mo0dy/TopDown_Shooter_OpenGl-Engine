#include "Animation.h"

Etex::Etex(std::string texture, GLfloat width) : tex(texture) {
	texSize.x = width;
	texSize.y = width * ResourceManager::GetTexture(texture).Height / ResourceManager::GetTexture(texture).Width;
}

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::LoadAnimation(std::string path, std::string filetype, GLint amount, GLfloat width, GLboolean alpha, std::string name) {
	std::string texName;
	for (int i = 0; i < amount; i++) {
		texName = name + "_T_" + std::to_string(i);
		ResourceManager::LoadTexture((path + "\\T" + std::to_string(i) + filetype).c_str(), alpha, texName);
		addETex(new Etex(texName, width));
	}
}

void Animation::startAnimation() {
	startTime = glfwGetTime();
	state = GL_TRUE;
}

void Animation::stopAnimation() {
	state = GL_FALSE;
}

Etex* Animation::getETex() {
	GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), animationTime);
	return textures[(GLint) (dt / animationTime * textures.size())];
}

void Animation::addETex(Etex* tex) {
	textures.push_back(tex);
}

GLint Animation::getSize() {
	return textures.size();
}

GLboolean Animation::getState() {
	return state && textures.size() > 0;
}