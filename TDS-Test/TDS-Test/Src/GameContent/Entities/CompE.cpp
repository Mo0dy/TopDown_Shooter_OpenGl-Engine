#include "CompE.h"

CompE::CompE(glm::vec2 position) : DynE(position)
{
}

CompE::~CompE()
{
	for (auto& x : subEntities) {
		delete x.second;
	}
}

GLboolean CompE::UpdateE(GLfloat dt)
{
	PreUpdate();
	UpdatePos(dt);
	UpdateSubE(dt);
	return glm::length(vel) > 0;
}

void CompE::UpdateSubE(GLfloat dt) 
{
	for (auto& x : subEntities) {
		x.second->UpdateE(dt);
	}
}

void CompE::UpdateAni() {
	animations[ani].UpdateAni(this, GL_FALSE);
	for (auto& x : subEntities) 
	{
		x.second->UpdateAni();
	}
}

void CompE::SetSubESize(glm::vec3 size, std::string name) 
{
	subEntities[name]->SetSize(size);
	// This will increase the point when we do hitbox collision checking to the biggest size. If you decrease the size it won't decrease with it! <-- Bug.
	if (glm::length(subEntities[name]->GetSize()) > glm::length(this->size))
	{
		this->size = subEntities[name]->GetSize();
	}
}

void CompE::SetSubESize(GLfloat width, std::string name) 
{
	subEntities[name]->SetSize(width);
	// This will increase the point when we do hitbox collision checking to the biggest size. If you decrease the size it won't decrease with it! <-- Bug.
	if (glm::length(subEntities[name]->Get2DSize()) > glm::length(this->size))
	{
		this->SetSize(subEntities[name]->Get2DSize());
	}
}

void CompE::SetColor(glm::vec3 color)
{
	for (auto& x : subEntities)
	{
		x.second->SetColor(color);
	}
}