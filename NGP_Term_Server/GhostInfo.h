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
	//�ͽ� ��ǥ
	float x;
	float y;
	float z;
	//�ͽ� �ӵ�
	float speed;
	//�ͽ� ����
	int direction;
	int type; //x�� ���� �ͽ�����, z�� ���� �ͽ�����
};
