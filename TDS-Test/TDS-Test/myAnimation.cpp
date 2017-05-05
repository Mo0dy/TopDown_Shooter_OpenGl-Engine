#include "myAnimation.h"

myAnimation::myAnimation()
{
}

myAnimation::~myAnimation()
{
}

void myAnimation::startAnimation() {
	startTime = glfwGetTime();
	state = GL_TRUE;
}

void myAnimation::stopAnimation() {
	state = GL_FALSE;
}

Etex* myAnimation::getETex() {
	GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), animationTime);
	return textures[(GLint) (dt / animationTime / textures.size())];
}

void myAnimation::addETex(Etex* tex) {
	textures.push_back(tex);
}

GLint myAnimation::getSize() {
	return textures.size();
}

GLboolean myAnimation::getState() {
	return state && textures.size() > 0;
}