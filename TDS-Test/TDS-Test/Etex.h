#pragma once

#include "Util.h"
#include "Texture2D.h"
#include "HitObject.h"

class Etex {
public:

	Etex();
	Etex(Texture2D texture, GLfloat width);
	virtual ~Etex();

	// Util
	void UpdateHObjs();
	void FitHObj(); // Fits the Hitobject to the texture

	// Getters and setters
	const Texture2D* const GetTex();
	glm::vec2 GetTexSize();
	void SetTexSize(glm::vec2 textureSize);
	void SetTexSize(GLfloat width); // Sets to the texture size to a given width and retains aspect ratio
	void AddHitbox(HitObject hObj);

protected:
	Texture2D tex;
	std::vector<HitObject> hObjs;
	glm::vec2 texSize;
};

