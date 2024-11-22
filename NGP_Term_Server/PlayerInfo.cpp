#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
	m_network = new Network;
	m_online = FALSE;
	m_id = 0;
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
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
	if (!m_network) {											// m_network�� nullptr���� Ȯ��
		printf("Error: m_network is not initialized.\n");
		return INVALID_SOCKET;									// ��ȿ���� ���� ���� ��ȯ
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
