#include "Etex.h"
#include "ResourceManager.h"

Etex::Etex()
{
}

Etex::Etex(Texture2D* texture, GLfloat width) : tex(texture) 
{
	SetTexSize(width);
}

Etex::~Etex()
{
}

void Etex::UpdateHObjs() 
{
	hComb.Clear();
	for (HitCircle rHC : rHComb.hitCircles) { hComb.hitCircles.push_back(HitCircle(rHC, this->texSize)); }
	for (HitPoly rHP : rHComb.hitPolys) { hComb.hitPolys.push_back(HitPoly(rHP, this->texSize)); }
	for (HitBox rHB : rHComb.hitBoxes) { hComb.hitBoxes.push_back(HitBox(rHB, this->texSize)); }
}

void Etex::FitHObj()
{
	rHComb.Clear();
	rHComb.hitBoxes.push_back(HitBox(glm::vec2(0), glm::vec2(1), 0));
	UpdateHObjs();
}

// Getters and setters
const Texture2D* const Etex::GetTex() const
{
	return tex;
}

glm::vec2 Etex::GetTexSize() const
{
	return texSize;
}

glm::vec2 Etex::GetTexSize(GLfloat width) const
{
 	return glm::vec2(width, (GLfloat) tex->Height / tex->Width * width);
}

HitComb Etex::GetHitComb() const 
{
	return hComb;
}

HitComb Etex::GetHitComb(glm::vec2 size) const { return HitComb(this->rHComb, size); }
HitComb Etex::GetHitComb(GLfloat width) const { return GetHitComb(glm::vec2(width, (GLfloat) tex->Height / tex->Width * width)); }

void Etex::SetTexSize(glm::vec2 textureSize)
{
	texSize = textureSize;
	UpdateHObjs();
}

void Etex::SetTexSize(GLfloat width)
{
	texSize = glm::vec2(width, (GLfloat) tex->Height / tex->Width * width);
	UpdateHObjs();
}

void Etex::AddRHitbox(HitBox rHBox) { rHComb.hitBoxes.push_back(rHBox); }
void Etex::SetRHComb(HitComb* rHComb) { this->rHComb = *rHComb; }
void Etex::SetTex(Texture2D* tex) { this->tex = tex; }