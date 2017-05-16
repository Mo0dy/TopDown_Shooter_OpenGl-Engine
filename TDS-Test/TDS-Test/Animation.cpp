#include "Animation.h"

Animation::Animation() {
}

Animation::Animation(std::string name, GLboolean repeat) : repeat(repeat), Etextures(ResourceManager::Animations[name])
{
}

Animation::Animation(std::string name, GLfloat width, GLboolean repeat) : Animation(name, repeat) {
	EnforceWidth(width);
}

Animation::~Animation()
{
}

void Animation::LoadFromRM(std::string name) 
{
	Etextures = ResourceManager::Animations[name];
}

void Animation::LoadFromRM(std::string name, GLfloat width) 
{
	LoadFromRM(name);
	EnforceWidth(width);
}

void Animation::EnforceWidth(GLfloat width) 
{
	for (Etex &e : Etextures) {
		e.SetTexSize(width);
	}
}

void Animation::Start() 
{
	startTime = glfwGetTime();
	state = GL_TRUE;
}

void Animation::Stop() 
{
	state = GL_FALSE;
}

Etex& Animation::GetETex() 
{
	if (startTime + aniTime < glfwGetTime() && !repeat) {
		state = GL_FALSE;
		Etextures.back();
	}
	else {
		GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), aniTime);
		return Etextures[(GLint)(dt / aniTime * Etextures.size())];
	}
}

void Animation::SetFPS(GLfloat fps) 
{
	aniTime = GetNumber() / fps;
}

void Animation::SetAniTime(GLfloat aniTime) 
{
	this->aniTime = aniTime;
}

Etex& Animation::GetETex(GLuint pos)
{
	return Etextures[pos];
}

GLint Animation::GetNumber() 
{
	return Etextures.size();
}

GLboolean Animation::GetState() 
{
	return state && Etextures.size() > 0;
}