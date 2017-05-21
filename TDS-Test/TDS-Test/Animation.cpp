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

void Animation::UpdateAni(Entity* e, GLboolean fitSize)
{
	if (state) {
		if (startTime + aniTime < glfwGetTime() && !repeat) {
			state = GL_FALSE;
			e->SetTex(Etextures.back().GetTex());
			e->SetSize(Etextures.back().GetTexSize());

			if (updateHObj) {
				if (fitSize) {
					e->SetHitComb(Etextures.back().GetHitComb(e->Get2DSize()));
				}
				else {
					e->SetHitComb(Etextures.back().GetHitComb());
				}
			}
		}
		else {
			GLfloat dt = glm::mod<GLfloat>((startTime - glfwGetTime()), aniTime);
			GLuint etexIndex = (GLuint)(dt / aniTime * Etextures.size());

			e->SetTex(Etextures[etexIndex].GetTex());
			e->SetSize(Etextures.back().GetTexSize());
			if (updateHObj) {
				if (fitSize) {
					e->SetHitComb(Etextures.back().GetHitComb(e->Get2DSize()));
				}
				else {
					e->SetHitComb(Etextures.back().GetHitComb());
				}
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