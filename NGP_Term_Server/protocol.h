#pragma once
#define _CRT_SECURE_NO_WARNINGS // 구형 C 함수 사용 시 경고 끄기
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 구형 소켓 API 사용 시 경고 끄기

#include <winsock2.h> // 윈속2 메인 헤더
#include <ws2tcpip.h> // 윈속2 확장 헤더

#include <tchar.h> // _T(), ...
#include <stdio.h> // printf(), ...
#include <stdlib.h> // exit(), ...
#include <string.h> // strncpy(), ...

#pragma comment(lib, "ws2_32") // ws2_32.lib 링크

#include <iostream>
#include "include/glew.h"
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

using namespace glm;

constexpr int MaxUser = 2;		//클라 맥스 유저 수
constexpr int MaxGhost = 20;	//귀신 맥스 수

enum CS_PacketType				// 클라이언트 -> 서버
{
	CS_PlayerCoord = 100,		//
	CS_DoorCheck = 101,			//
	CS_GOTKEY = 102,
	CS_GOTITEM = 103,
	CS_Ending = 104,
	CS_GhostCoord = 105
};

enum SC_PacketType				// 서버 -> 클라이언트
{
	SC_EnterId = 0,				// 입장id
	SC_AnotherCoord = 1,		// 상대방 좌표 패킷
	SC_DoorCheck = 2,			// 문 상태
	SC_GOTKEY = 3,			// 키 패킷
	SC_GOTITEM = 4,			// 아이템 패킷
	SC_Ending = 5,
	SC_GhostCoord = 6
};


// 1. 플레이어 좌표
// 2. 아이템 위치
// 3. 키 위치
// 4. 문 열림 정도
// 5. 유령 좌표
// 6. 게임 종료 패킷

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
	glm::vec3 cameraAt;
};

struct PlayerCoordPacket {
	char type;
	char size;
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
	bool IsUsed; //사용 되었는지
	int ability; //무슨 아이템인지
};
struct GotKeyPacket {
	char type;
	int key_num;
	int HowManyKey;
	//bool IsFind; //찾았는지
};
struct GotItemPacket {
	char type;
	int item_num;
};
struct SC_DoorOpenPacket {
	char type;
	int num; // 문 번호
	bool value; // 조건체크
};
struct CS_DoorOpenPacket {
	char type;
	int num; // 문 번호
	bool value; // 조건체크
};
struct GhostCoordPacket {
	float X;
	float y;
	float z;
	int direction; //귀신이 바라보는 방향
};

struct GhostCoordPacketAll {
	char type;
	int ghostCount;                   // 귀신 개수
	GhostCoordPacket ghosts[20];      // 모든 귀신의 정보
};

struct Ghostcheck {
	char type;
	int id;
};

struct GameOverPacket {
	char size;
	char type;
};
struct SC_GameOverPacket {
	char type;
	bool value;
};
struct CS_GameOverPacket {
	char type;
	bool value;
};