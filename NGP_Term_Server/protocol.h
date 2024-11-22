#pragma once
#define _CRT_SECURE_NO_WARNINGS // ���� C �Լ� ��� �� ��� ���
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���� ���� API ��� �� ��� ���

#include <winsock2.h> // ����2 ���� ���
#include <ws2tcpip.h> // ����2 Ȯ�� ���

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib ��ũ

#include <iostream>
#include "include/glew.h" // �ʿ��� ������� include
#include "include/freeglut.h"
#include "include/freeglut_ext.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/ext.hpp"
#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")
#pragma comment(lib, "glew32s")


#define SERVERPORT 9000
#define BUFSIZE    512

constexpr int MaxUser = 2;		//Ŭ�� �ƽ� ���� ��

enum CS_PacketType				// Ŭ���̾�Ʈ -> ����
{

};

enum SC_PacketType				// ���� -> Ŭ���̾�Ʈ
{
	SC_EnterId = 0,				// ����id
	SC_AnotherCoord = 1			// ���� ��ǥ ��Ŷ
};

// 1. �÷��̾� ��ǥ
// 2. ������ ��ġ
// 3. Ű ��ġ
// 4. �� ���� ����
// 5. ���� ��ǥ
// 6. ���� ���� ��Ŷ

struct SC_EnterIdPacket {
	char type;
	int id;
};

struct SC_AnotherPlayerCoordPacket {
	char type;
	int id;
	float x;
	float y;
	float z;
};

struct PlayerCoordPacket {
	char size;
	char type;
	int id;
	float x;
	float y;
	float z;
	glm::vec3 cameraAt;
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