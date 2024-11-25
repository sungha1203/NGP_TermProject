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

#define SERVERPORT 9000
#define BUFSIZE    512

constexpr int MaxUser = 2;		//클라 맥스 유저 수

enum CS_PacketType				// 클라이언트 -> 서버
{

};

enum SC_PacketType				// 서버 -> 클라이언트
{
	SC_EnterId = 0,				// 입장id
	SC_AnotherCoord = 1			// 상대방 좌표 패킷
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
	bool IsUsed; //사용 되었는지
	int ability; //무슨 아이템인지
};
struct KeyCoordPacket {
	char size;
	char type;
	float X;
	float y;
	float z;
	bool IsFind; //찾았는지
};
struct DoorOpenPacket {
	char size;
	char type;
	int num; // 문 번호
	bool value; // 조건체크
};
struct GhostCoordPacket {
	char size;
	char type;
	float X;
	float y;
	float z;
	int num; // 귀신 번호
};
struct GameOverPacket {
	char size;
	char type;
};