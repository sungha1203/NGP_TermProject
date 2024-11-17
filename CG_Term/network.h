#pragma once
#include "protocol.h"

class Network
{
	SOCKET sock;

public:
	Network();
	~Network();

	bool IsConnect();
	void SendPacket();
};