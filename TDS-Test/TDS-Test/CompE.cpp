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

void CompE::updateSupE() {
	for (const auto& x : subEntities) {
		x.second->pos = pos + x.second->rPos;
		x.second->angle = angle + x.second->rAngle;
	}
}

void CompE::combineHitboxes() {
	// Maybe this should only be done if changes happen?
	Hitboxes.clear();
	for (auto const& x : subEntities) {
		for (Hitbox *h : x.second->Hitboxes) {
			this->Hitboxes.push_back(h);
			this->Hitboxes.back()->pos += x.second->rPos;
			this->Hitboxes.back()->angle += x.second->rAngle;
		}
	}
}

void CompE::setColor(glm::vec3 color) {
	for (auto const& x : subEntities) {
		x.second->color = color;
	}
}
std::map <std::string, SubE*> CompE::getSubE() { return subEntities; }