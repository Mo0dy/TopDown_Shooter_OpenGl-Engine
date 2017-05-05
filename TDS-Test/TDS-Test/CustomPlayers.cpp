#include "CustomPlayers.h"

#include "Renderer.h"

Robot::Robot(glm::vec2 position): Player(position) 
{
	inherentForce = 2500;
	movState = STOPPING;
	sprintMod = 4;
	turnSpeed = 10;

	mass = 80;
	airFricCoeff = -100; // substitues for other resistances
	dynFricCoeff = -3;
	statFricCoeff = -5;

	addEntities.push_back(new Entity(pos));
}

Robot::~Robot()
{
}

GLboolean Robot::updateE(GLfloat dt) {
	// updating animation
	//if (ani.getState()) {
	//	tex = ani.getETex()->tex;
	//	size = ani.getETex()->texSize;
	//	if (ani.getETex()->hitboxes.size() > 0) {
	//		Hitboxes = ani.getETex()->hitboxes;
	//	}
	//}

	force += airRes();

	if (glm::length(movDir) > 0) {
		movDir = glm::normalize(movDir);
	}

	switch (movState) {
	case STOPPING: force += fricRes();
		break;
	case RUNNING: force += movDir * inherentForce;
		break;
	case SPRINTING: force += movDir * inherentForce * sprintMod;
		break;
	}

	glm::vec2 dV = dt * force / mass;

	// safeguard for wiggeling close to 0v
	if (glm::length(vel) > 0 && vel.x * (vel.x + dV.x) <= 0 && vel.y * (vel.y + dV.y) <= 0) {
		vel = glm::vec2(0, 0);
	} 
	else {
		vel += dV; // acceleration is in m/s^2 so we need to divide my dt to get a velocity
	}

	pos += dt * vel; // vel ist in m/s so if multiplied by a time in second we will get the change in distance during that time;

	setTrackAngle(dt);

	addEntities[Robot::TRACKS]->pos = pos;

	force = glm::vec2(1500, 1500);

	Renderer::drawLineBuffer.push_back(myVertex(pos, glm::vec3(1.0f, 1.0f, 0.0f)));
	Renderer::drawLineBuffer.push_back(myVertex(pos + force * 0.001f, glm::vec3(1.0f, 1.0f, 0.0f)));

	force = glm::vec2(0, 0);
	movDir = glm::vec2(0, 0);
	return glm::length(vel) > 0;
}

void Robot::setTrackAngle(GLfloat dt) {
	GLfloat gAngle = addEntities[Robot::TRACKS]->angle; // The angle the player wants to have

								  // you have to check the if vel is longer then 0 otherwise you will divide by 0 while normalizing
	if (glm::length(vel) > 0) {
		// this returns the absolute angle between the y axix and the player ?negative if counter clockwise rotation positive if clockwise? <-- not sure about this you have to try it out
		if (vel.x > 0) {
			gAngle = -acos(glm::dot(glm::normalize(vel), glm::vec2(0, 1))); // shortest angle between the y axis and the velocity vector (negative)
		}
		else {
			gAngle = acos(glm::dot(glm::normalize(vel), glm::vec2(0, 1))); // shortest angle between the y axis and the velocity vector (positive)
		}
	}

	// I would propose that you only change the stuff below this comment
	if (gAngle > addEntities[Robot::TRACKS]->angle) {
		addEntities[Robot::TRACKS]->angle += dt * turnSpeed;
		if (addEntities[Robot::TRACKS]->angle > gAngle) {
			addEntities[Robot::TRACKS]->angle = gAngle;
		}
	}
	else if (gAngle < addEntities[Robot::TRACKS]->angle) {
		addEntities[Robot::TRACKS]->angle -= dt * turnSpeed;
		if (addEntities[Robot::TRACKS]->angle < gAngle) {
			addEntities[Robot::TRACKS]->angle = gAngle;
		}
	}
}