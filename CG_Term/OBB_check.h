#pragma once

struct ST_OBB
{
	glm::vec3 vCenterPos;
	glm::vec3 vAxisDir[3];
	float fAxisLen[3];
};

bool CheckOBBCollision(ST_OBB* Box1, ST_OBB* Box2);

