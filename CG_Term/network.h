#pragma once
#include "protocol.h"

class Network
{
	SOCKET sock;

public:
	Network();
	~Network();

	SOCKET getsock();                        
	bool IsConnect();                        
	void SendPacket(char* packet, int size); 
};