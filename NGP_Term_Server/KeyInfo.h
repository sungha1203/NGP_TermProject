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
	int key[3] = { 0 };//Ű ��Ծ����� Ȯ�ο�
	int keyNum = -1;//��� Ŭ�� ���� Ű �ѹ�
	KeyInfo();
	~KeyInfo();
	int GetHowManyKey();
};