#pragma once
#include "protocol.h"

struct keyNetwork {
	SOCKET client_socket;
};


class KeyInfo
{
private:
	keyNetwork* m_network;
public:
	int key[3] = { 0 };//키 몇개먹었는지 확인용
	int keyNum = -1;//방금 클라가 먹은 키 넘버
	KeyInfo();
	~KeyInfo();
	int GetHowManyKey();
};