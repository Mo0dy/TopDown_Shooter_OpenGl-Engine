#pragma once

#include "Util.h"

#include "Texture2D.h"
#include "Animation.h"
#include "HitObject.h"

class Entity
{
public:
	Entity();
	Entity(glm::vec2 position);
	Entity(glm::vec2 position, Texture2D texture);
	Entity(glm::vec2 position, GLfloat angle, Texture2D texture);
	Entity(glm::vec2 position, Animation ani, std::string aniName);
	Entity(glm::vec2 position, GLfloat angle, Animation ani, std::string aniName);

	virtual ~Entity();

	Texture2D tex;
	std::vector<HitObject> hitboxes;
	std::map<std::string, Animation> animations;
	std::string ani; // The name of the currently Played Animation

	virtual GLboolean UpdateE(GLfloat dt); // Updating function will get called every frame
	virtual void SetColor(glm::vec3 color);
	virtual void UpdateAni(); // Updates texture and hitbox from the animation

	GLboolean GetErase();
	glm::vec2 GetPos();
	GLfloat GetAngle();
	glm::vec3 GetColor();

protected:
	GLboolean erase;
	glm::vec2 pos; // The position in WCS coordinates
	GLfloat angle; // The angle of the entitie in radians
	glm::vec3 color; // The color will be overlayed on the texture
};

