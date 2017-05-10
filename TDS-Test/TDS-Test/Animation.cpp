#include "Animation.h"

std::map<std::string, std::vector<Etex*> > Animation::Animations;

Etex::Etex(std::string texture, GLfloat width) : tex(texture) {
	texSize.x = width;
	texSize.y = width * ResourceManager::GetTexture(texture).Height / ResourceManager::GetTexture(texture).Width;
}

Animation::Animation(std::string name, GLboolean repeat): name(name), repeat(repeat)
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
		Animations[name].push_back(new Etex(texName, width));
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
	if (startTime + animationTime < glfwGetTime() && !repeat) {	
		state = GL_FALSE;
		return Animations[name].back();
	}
	else {
		GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), animationTime);
		return Animations[name][(GLint)(dt / animationTime * Animations[name].size())];
	}
}

void Animation::setFPS(GLfloat fps) {
	animationTime = getSize() / fps;
}

GLint Animation::getSize() {
	return Animations[name].size();
}

GLboolean Animation::getState() {
	return state && Animations[name].size() > 0;
}