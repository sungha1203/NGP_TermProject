#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ����
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ����

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#define SERVERPORT 9000
#define BUFSIZE    512

constexpr int MaxUser = 2;		//Ŭ�� �ƽ� ���� ��

// 1. �÷��̾� ��ǥ
// 2. ������ ��ġ
// 3. Ű ��ġ
// 4. �� ���� ����
// 5. ���� ��ǥ
// 6. ���� ���� ��Ŷ

struct PlayerCoordPacket {
	char size;
	char type;
	vec3 cameraAt;
	float x;
	float y;
	float z;
};
struct ItemCoordPacket {
	char size;
	char type;
	float X;
	float y;
	float z;
	bool IsUsed; //��� �Ǿ�����
	int ability; //���� ����������
};
struct KeyCoordPacket {
	char size;
	char type;
	float X;
	float y;
	float z;
	bool IsFind; //ã�Ҵ���
};
struct DoorOpenPacket {
	char size;
	char type;
	int num; // �� ��ȣ
	bool value; // ����üũ
};
struct GhostCoordPacket {
	char size;
	char type;
	float X;
	float y;
	float z;
	int num; // �ͽ� ��ȣ
};
struct GameOverPacket {
	char size;
	char type;
};