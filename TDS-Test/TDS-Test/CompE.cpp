#include "CompE.h"

CompE::CompE(glm::vec2 position) : DynE(position)
{
}

CompE::~CompE()
{
	// This is buggy for some reason
	//for (const auto& x : subEntities) {
	//	delete x.second;
	//}
}

void CompE::UpdateSubE() {
	for (const auto& x : subEntities) {
		x.second->pos = this->pos + x.second->rPos;
		x.second->angle = this->angle + x.second->rAngle;
	}
}

void CompE::CombineHitboxes() {
	// Maybe this should only be done if changes happen?
	hitboxes.clear();
	for (auto const& x : subEntities) {
		for (Hitbox *h : x.second->hitboxes) {
			this->hitboxes.push_back(h);
			this->hitboxes.back()->pos = x.second->rPos;
			this->hitboxes.back()->angle = x.second->rAngle; // <- problem is here !!!!
		}
	}
}

void CompE::UpdateAni() {
	for (auto const& x : subEntities) {
		x.second->UpdateAni();
	}
}

void CompE::SetSubESize(glm::vec2 size, std::string name) {
	if (glm::length(size)) {
		this->etex.setTexSize(size);
	}
	subEntities[name]->etex.setTexSize(size);
}

void CompE::SetSubESize(GLfloat width, std::string name) {
	subEntities[name]->etex.setTexSize(width);
	if (glm::length(subEntities[name]->etex.getTexSize()) > glm::length(this->etex.getTexSize())) {
		this->etex.setTexSize(subEntities[name]->etex.getTexSize());
	}
}

void CompE::SetColor(glm::vec3 color) {
	for (auto const& x : subEntities) {
		x.second->color = color;
	}
}

std::map <std::string, SubE*> CompE::GetSubE() { return subEntities; }