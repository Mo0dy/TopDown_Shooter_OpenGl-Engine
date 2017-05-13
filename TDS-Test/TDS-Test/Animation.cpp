#include "Animation.h"

Animation::Animation() {
}

Animation::Animation(std::string name, GLboolean repeat) : repeat(repeat), Etextures(ResourceManager::Animations[name])
{
}

Animation::Animation(std::string name, GLfloat width, GLboolean repeat) : Animation(name, repeat) {
	enforceWidth(width);
}

Animation::~Animation()
{
}

void Animation::load(std::string name) {
	Etextures = ResourceManager::Animations[name];
}

void Animation::load(std::string name, GLfloat width) {
	load(name);
	enforceWidth(width);
}

void Animation::enforceWidth(GLfloat width) {
	for (Etex &e : Etextures) {
		e.setTexSize(width);
	}
}

void Animation::startAnimation() {
	startTime = glfwGetTime();
	state = GL_TRUE;
}

void Animation::stopAnimation() {
	state = GL_FALSE;
}

Etex& Animation::getETex() {
	if (startTime + animationTime < glfwGetTime() && !repeat) {
		state = GL_FALSE;
		Etextures.back();
	}
	else {
		GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), animationTime);
		return Etextures[(GLint)(dt / animationTime * Etextures.size())];
	}
}

Etex& Animation::getETex(GLuint pos) {
	return Etextures[pos];
}

void Animation::setFPS(GLfloat fps) {
	animationTime = getSize() / fps;
}

GLint Animation::getSize() {
	return Etextures.size();
}

GLboolean Animation::getState() {
	return state && Etextures.size() > 0;
}