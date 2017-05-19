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
	Entity(glm::vec2 position, GLfloat angle);
	Entity(glm::vec2 position, const Texture2D* texture);
	Entity(glm::vec2 position, GLfloat angle, const Texture2D* texture);
	Entity(glm::vec2 position, class Animation ani, std::string aniName);
	Entity(glm::vec2 position, GLfloat angle, class Animation ani, std::string aniName);

	Entity(const Etex* etex, GLfloat width);
	Entity(const Etex* etex, glm::vec2 size);

	virtual ~Entity();

	std::vector<HitObject> hitObjs;
	std::map<std::string, class Animation> animations;
	std::string ani; // The name of the currently Played Animation

	virtual GLboolean UpdateE(GLfloat dt); // Updating function will get called every frame
	virtual void SetColor(glm::vec3 color);
	virtual void UpdateAni(); // Updates texture and hitbox from the animation

	virtual void Collision(Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithStat(Entity* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithDyn(class DynE* dE, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithEnemy(class Enemy* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithPlayer(class Player* p, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithSubE(class SubE* sE, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithESubE(class SubE* e, GLfloat penDepth, glm::vec2 colAxis);
	virtual void ColWithPSubE(class SubE* p, GLfloat penDepth, glm::vec2 colAxis);


	virtual void GetAttacked(GLfloat damage);

	GLboolean GetErase() const;
	glm::vec2 GetPos() const;
	glm::vec2 GetSize() const;
	GLfloat GetAngle() const;
	glm::vec3 GetColor() const;
	const Texture2D* GetTex() const;

	void SetTex(const Texture2D *tex);
	void SetSize(glm::vec2 size);
	void SetSize(GLfloat width);

protected:
	const Texture2D *tex;
	GLboolean erase;
	glm::vec2 pos; // The position in WCS coordinates
	glm::vec2 size;
	GLfloat angle; // The angle of the entitie in radians
	glm::vec3 color; // The color will be overlayed on the texture
};

