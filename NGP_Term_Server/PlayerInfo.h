#pragma once
#include "protocol.h"

struct Network {
	SOCKET client_socket;
};

class PlayerInfo
{
private:
	Network* m_network;
	int m_id;				// Ŭ���̾�Ʈ ��ȣ
	float m_x, m_y, m_z;	// �÷��̾� ��ǥ
	bool m_online;			// �¶������� �ƴ���
	vec3 m_cameraAt;

public:
	PlayerInfo();
	~PlayerInfo();

public:
	void SetSocket(SOCKET socket);
	void SetId(int num);
	void SetCoord(float x, float y, float z);
	void SetOnline();
	void SetCameraAt(vec3 cameraAt);

	SOCKET GetSocket();
	int GetId();
	float GetCoordX();
	float GetCoordY();
	float GetCoordZ();
	bool AreUOnline();
	vec3 GetCameraAt();
};
