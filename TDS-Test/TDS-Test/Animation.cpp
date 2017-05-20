#include "Animation.h"

Animation::Animation() {
}

Animation::Animation(std::string name, GLboolean repeat, GLboolean updateHObj) : repeat(repeat), updateHObj(updateHObj), Etextures(ResourceManager::GetAnimation(name))
{
}

Animation::Animation(std::string name, GLfloat width, GLboolean repeat, GLboolean updateHObj) : Animation(name, repeat, updateHObj) {
	this->EnforceWidth(width);
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

void Animation::UpdateAni(Entity* masterE)
{
	if (state) {
		if (startTime + aniTime < glfwGetTime() && !repeat) {
			state = GL_FALSE;
			masterE->SetTex(Etextures.back().GetTex());
			masterE->Set2DSize(Etextures.back().GetTexSize());

			if (updateHObj) {
				masterE->hitObjs = Etextures.back().GetHitObjs(masterE->Get2DSize());
			}
		}
		else {
			GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), aniTime);
			GLuint etexIndex = (GLuint)(dt / aniTime * Etextures.size());

			masterE->SetTex(Etextures[etexIndex].GetTex());
			masterE->Set2DSize(Etextures.back().GetTexSize());
			if (updateHObj) {
				masterE->hitObjs = Etextures[etexIndex].GetHitObjs(masterE->Get2DSize());
			}
		}
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

Etex& Animation::GetETex(GLuint index)
{
	return Etextures[index];
}

GLint Animation::GetNumber()
{
	return Etextures.size();
}

GLboolean Animation::GetState()
{
	return state && Etextures.size() > 0;
}