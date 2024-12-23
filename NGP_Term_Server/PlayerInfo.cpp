﻿#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	m_network = new Network;
	m_online = FALSE;
	m_id = 0;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_cameraAt = vec3(0.0f, 0.0f, 0.0f);
	m_door1 = false;
	m_door2 = false;
}

PlayerInfo::~PlayerInfo() 
{
	//delete m_network;
}

//////////////////////////////////////////////////////////

void PlayerInfo::SetSocket(SOCKET socket)
{
	m_network->client_socket = socket;
}

void PlayerInfo::SetId(int id)
{
	m_id = id;
};

void PlayerInfo::SetCoord(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void PlayerInfo::SetOnline()
{
	m_online = TRUE;
}

void PlayerInfo::SetCameraAt(vec3 cameraAt)
{
	m_cameraAt = cameraAt;
}

void PlayerInfo::SetDoor1Check(bool doorcheck)
{
	m_door1 = doorcheck;
}

void PlayerInfo::SetDoor2Check(bool doorcheck)
{
	m_door2 = doorcheck;
}

//////////////////////////////////////////////////////////

float PlayerInfo::GetCoordX()
{
	return m_x;
}

float PlayerInfo::GetCoordY()
{
	return m_y;
}

float PlayerInfo::GetCoordZ()
{
	return m_z;
}

SOCKET PlayerInfo::GetSocket()
{
	if (!m_network) {											// m_network가 nullptr인지 확인
		printf("Error: m_network is not initialized.\n");
		return INVALID_SOCKET;									// 유효하지 않은 소켓 반환
	}
	return m_network->client_socket;
}

int PlayerInfo::GetId()
{
	return m_id;
}

bool PlayerInfo::AreUOnline()
{
	return m_online;
}

vec3 PlayerInfo::GetCameraAt()
{
	return m_cameraAt;
}

bool PlayerInfo::GetDoor1Check()
{
	return m_door1;
}

bool PlayerInfo::GetDoor2Check()
{
	return m_door2;
}