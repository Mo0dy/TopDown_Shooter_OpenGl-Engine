#include "Etex.h"
#include "ResourceManager.h"

Etex::Etex()
{
}

Etex::Etex(Texture2D texture, GLfloat width) : tex(texture) 
{
	SetTexSize(width);
}

Etex::~Etex()
{
}

void Etex::UpdateHObjs() 
{
	hObjs.clear();
	for (HitObject rHO : rHObjs) {
		hObjs.push_back(HitObject(texSize * rHO.GetPos * 0.5f, texSize * rHO.GetSize(), rHO.GetAngle()));
	}
}

void Etex::FitHObj()
{
	rHObjs.clear();
	rHObjs.push_back(HitObject(glm::vec2(1), glm::vec2(1), 0));
	UpdateHObjs();
}

// Getters and setters
const Texture2D* const Etex::GetTex() const
{
	return &tex;
}

glm::vec2 Etex::GetTexSize() const
{
	return texSize;
}

std::vector<HitObject> Etex::GetHitObjs() const 
{
	return hObjs;
}

void Etex::SetTexSize(glm::vec2 textureSize)
{
	texSize = textureSize;
	UpdateHObjs();
}

void Etex::SetTexSize(GLfloat width)
{
	texSize = glm::vec2(width, tex.Height / tex.Width * width);
	UpdateHObjs();
}

void Etex::AddRHitbox(HitObject rHObj)
{
	rHObjs.push_back(rHObj);
}