#pragma once
#include "protocol.h"

struct Network {
	SOCKET client_socket;
};

class PlayerInfo
{
private:
	Network* m_network;
	int m_id;				// 클라이언트 번호
	float m_x, m_y, m_z;	// 플레이어 좌표
	bool m_online;			// 온라인인지 아닌지
	vec3 m_cameraAt;
	bool m_door1;
	bool m_door2;

public:
	PlayerInfo();
	~PlayerInfo();

public:
	void SetSocket(SOCKET socket);
	void SetId(int num);
	void SetCoord(float x, float y, float z);
	void SetOnline();
	void SetCameraAt(vec3 cameraAt);
	void SetDoor1Check(bool doorcheck);
	void SetDoor2Check(bool doorcheck);

	SOCKET GetSocket();
	int GetId();
	float GetCoordX();
	float GetCoordY();
	float GetCoordZ();
	bool AreUOnline();
	vec3 GetCameraAt();
	bool GetDoor1Check();
	bool GetDoor2Check();
};