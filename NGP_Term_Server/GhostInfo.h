#pragma once
#include "protocol.h"

class GhostInfo {
public:
	GhostInfo(int num);
	~GhostInfo();

	float GetCoordX();
	float GetCoordY();
	float GetCoordZ();
	int GetDirection();

	void move(int movetype);

private:
	//귀신 좌표
	float x;
	float y;
	float z;
	//귀신 속도
	float speed;
	//귀신 방향
	int direction;
	int type; //x축 방향 귀신인지, z축 방향 귀신인지
};
