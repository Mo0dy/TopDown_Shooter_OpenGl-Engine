#pragma once

#include "Util.h"
#include "Texture2D.h"
#include "HitObject.h"

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
	std::vector<HitObject> GetHitObjs() const;
	std::vector<HitObject> GetHitObjs(glm::vec2 size);
	std::vector<HitObject> GetHitObjs(GLfloat size);
	void SetTexSize(glm::vec2 textureSize);
	void SetTexSize(GLfloat width); // Sets to the texture size to a given width and retains aspect ratio
	void AddRHitbox(HitObject rHObj);
	void SetRHObjs(std::vector<HitObject*> rHObjs);
	void SetTex(Texture2D* tex);

protected:
	Texture2D* tex;
	std::vector<HitObject> rHObjs; // relative hitObjects
	std::vector<HitObject> hObjs;
	glm::vec2 texSize;
};

