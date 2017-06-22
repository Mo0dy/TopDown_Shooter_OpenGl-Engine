#pragma once

#include "..\Tools\Util.h"
#include "Texture2D.h"
#include "..\Maths\HitObject.h"

class Etex {
public:

	Etex();
	Etex(Texture2D* texture, GLfloat width);
	virtual ~Etex();

	// Util
	void UpdateHObjs();
	void FitHObj(); // Fits the Hitobject to the texture

	// Getters and setters
	const Texture2D* const GetTex() const;
	glm::vec2 GetTexSize() const;
	glm::vec2 GetTexSize(GLfloat width) const;
	HitComb GetHitComb() const;
	HitComb GetHitComb(glm::vec2 size) const;
	HitComb GetHitComb(GLfloat width) const;
	void SetTexSize(glm::vec2 textureSize);
	void SetTexSize(GLfloat width); // Sets to the texture size to a given width and retains aspect ratio
	void AddRHitbox(HitBox rHbox);
	void SetRHComb(HitComb* rHComb);
	void SetTex(Texture2D* tex);

protected:
	Texture2D* tex;
	HitComb rHComb; // relative hitObjects
	HitComb hComb;
	glm::vec2 texSize;
};

