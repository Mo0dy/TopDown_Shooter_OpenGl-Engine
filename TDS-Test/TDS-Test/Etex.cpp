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

}
void Etex::FitHObj()
{

}

// Getters and setters
const Texture2D* const Etex::GetTex()
{
	return &tex;
}
glm::vec2 Etex::GetTexSize()
{
	return texSize;
}
void Etex::SetTexSize(glm::vec2 textureSize)
{
	texSize = textureSize;
}
void Etex::SetTexSize(GLfloat width)
{
	texSize = glm::vec2(width, tex.Height / tex.Width * width);
}
void Etex::AddHitbox(HitObject hObj)
{
	hObjs.push_back(hObj)
}