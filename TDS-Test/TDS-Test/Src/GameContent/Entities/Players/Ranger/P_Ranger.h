#pragma once

#include "..\Player.h"

class P_Ranger : public Player
{
public:
	enum MOV_STATE {
		AIMING,
		NORMAL,
		SPRINTING
	};

	static void LoadRanger();

	P_Ranger(glm::vec2 pos);
	~P_Ranger();

	GLboolean UpdateE(GLfloat dt);

private:
	GLboolean PreUpdate();

	MOV_STATE movState;

	glm::vec2 movDir;
	glm::vec2 bodyDir;
};

