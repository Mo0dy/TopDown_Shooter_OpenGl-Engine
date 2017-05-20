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

	Entity(const Etex* etex, GLfloat width, GLfloat height);
	Entity(const Etex* etex, glm::vec3 size);

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
	glm::vec2 Get2DPos() const;
	glm::vec2 Get2DSize() const;
	glm::vec3 GetPos() const;
	glm::vec3 GetSize() const;
	GLfloat GetZPos() const; // Returns height over ground level of lowest part
	GLfloat GetZSize() const; // Returns size in Z dir
	GLfloat GetAngle() const;
	glm::vec3 GetColor() const;
	const Texture2D* GetTex() const;

	void SetTex(const Texture2D *tex);
	void Set2DSize(glm::vec2 size);
	void Set2DSize(GLfloat width);
	void SetSize(glm::vec3 size);
	void SetZSize(GLfloat zSize);

protected:
	const Texture2D *tex;
	GLboolean erase;
	glm::vec3 pos; // The position in WCS coordinates
	glm::vec3 size;
	GLfloat angle; // The angle of the entitie in radians
	glm::vec3 color; // The color will be overlayed on the texture
};

