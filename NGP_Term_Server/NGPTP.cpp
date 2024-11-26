#include "error.h"
#include "protocol.h"
#include "PlayerInfo.h"
#include "KeyInfo.h"
#include <vector>
#include <thread>
#include "chrono"
#include <mutex>

std::mutex g_playerMutex;

DWORD WINAPI ClientThread(LPVOID socket);
DWORD WINAPI SendPacket(LPVOID IpParam);

std::vector<PlayerInfo> g_player(MaxUser);
KeyInfo g_key;
int g_clientNum{};

void PlayerCollision();
void CheckPlayersArrivalAtDoor();
void BroadcastKeyState();
void BroadcastItemState(int item_num);
void SetCursorPosition(int y) {
	COORD coord = { 0, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
	int retval;

	for (int i = 0; i < MaxUser; ++i) {
		g_player[i].SetSocket(INVALID_SOCKET);
	}

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	/*HANDLE SendThread;
	SendThread = CreateThread(NULL, 0, SendPacket, 0, 0, 0);
	if (SendThread == NULL) {
		closesocket(listen_sock);
	}
	else {
		CloseHandle(SendThread);
	}*/

	// 데이터 통신에 사용할 변수
	struct sockaddr_in clientaddr;
	HANDLE hThread;

	while (g_clientNum <= MaxUser)
	{
		// accept()
		int addrlen = sizeof(clientaddr);
		SOCKET client_socket = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_socket == INVALID_SOCKET) {
			err_display("accept()");
			continue;
		}

		if (client_socket != INVALID_SOCKET) {
			std::lock_guard<std::mutex> lock(g_playerMutex);
			g_player[g_clientNum].SetSocket(client_socket);     // 소켓 설정
			g_player[g_clientNum].SetOnline();                  // 온라인 활성화
			g_player[g_clientNum].SetId(g_clientNum + 1);       // ID 부여

			// 접속한 클라이언트 정보 출력
			char addr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
			printf("\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d, 클라이언트 번호 = %d번 플레이어\n",
				addr, ntohs(clientaddr.sin_port), g_player[g_clientNum].GetId());

			// 클라이언트에게 접속 알림 패킷 전송
			{
				int len = sizeof(SC_EnterIdPacket);
				SC_EnterIdPacket* packet = new SC_EnterIdPacket;
				packet->type = SC_EnterId;
				packet->id = g_clientNum + 1;
				send(client_socket, reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(client_socket, reinterpret_cast<char*>(packet), len, 0);
				delete packet;
			}

			// 클라이언트를 처리할 스레드 생성
			HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)client_socket, 0, NULL);
			if (hThread == NULL) {
				closesocket(client_socket);
			}
			else {
				CloseHandle(hThread);
			}

			// 클라이언트 수 증가
			++g_clientNum;
			printf("현재 접속 클라이언트 수 : %d / %d\n", g_clientNum, MaxUser);
		}
		else {
			// accept 실패 시 오류 메시지 출력
			err_display("accept() 실패: 클라이언트 소켓이 유효하지 않습니다.");
		}
	}

	// 소켓 닫기
	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

DWORD WINAPI ClientThread(LPVOID socket)
{
	//PlayerInfo* sock = reinterpret_cast<PlayerInfo*>(socket);
	//SOCKET client_sock = sock->GetSocket();
	SOCKET client_sock = reinterpret_cast<SOCKET>(socket);

	int len;
	char buf[BUFSIZE];

	while (1) {
		// 데이터 길이 수신
		recv(client_sock, (char*)(&len), sizeof(int), MSG_WAITALL);
		recv(client_sock, buf, len, MSG_WAITALL);

		// 수신된 데이터를 구조체로 변환
		switch (buf[0]) {
		case CS_PlayerCoord:
		{
			PlayerCoordPacket* packet = reinterpret_cast<PlayerCoordPacket*>(buf);
			if (packet->id == 1) {   // 1번 플레이어 좌표 받기
				SetCursorPosition(7);
				//printf("%d번 플레이어의 좌표: x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->x, packet->y, packet->z);
				printf("%d번 플레이어의 방향: x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->cameraAt.x, packet->cameraAt.y, packet->cameraAt.z);
				g_player[packet->id - 1].SetCoord(packet->x, packet->y, packet->z);
				g_player[packet->id - 1].SetCameraAt(packet->cameraAt);
				{
					// 2번 좌표 1번한테 보내주기
					len = sizeof(SC_AnotherPlayerCoordPacket);
					SC_AnotherPlayerCoordPacket* packet2 = new SC_AnotherPlayerCoordPacket;
					packet2->type = SC_AnotherCoord;
					packet2->id = g_player[1].GetId();
					packet2->x = g_player[1].GetCoordX();
					packet2->y = g_player[1].GetCoordY();
					packet2->z = g_player[1].GetCoordZ();
					packet2->cameraAt = g_player[1].GetCameraAt();
					send(client_sock, reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(client_sock, reinterpret_cast<char*>(packet2), len, 0);
					delete packet2;
				}
			}
			else if (packet->id == 2)					// 2번 플레이어 좌표 받기
			{
				SetCursorPosition(9);
				//printf("%d번 플레이어의 좌표: x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->x, packet->y, packet->z);
				printf("%d번 플레이어의 방향: x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->cameraAt.x, packet->cameraAt.y, packet->cameraAt.z);
				g_player[packet->id - 1].SetCoord(packet->x, packet->y, packet->z);
				g_player[packet->id - 1].SetCameraAt(packet->cameraAt);
				{
					// 1번 좌표 2번한테 보내주기
					len = sizeof(SC_AnotherPlayerCoordPacket);
					SC_AnotherPlayerCoordPacket* packet3 = new SC_AnotherPlayerCoordPacket;
					packet3->type = SC_AnotherCoord;
					packet3->id = g_player[0].GetId();
					packet3->x = g_player[0].GetCoordX();
					packet3->y = g_player[0].GetCoordY();
					packet3->z = g_player[0].GetCoordZ();
					packet3->cameraAt = g_player[0].GetCameraAt();
					send(client_sock, reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(client_sock, reinterpret_cast<char*>(packet3), len, 0);
					delete packet3;
				}
			}
			else {
				printf("알 수 없는 패킷 또는 잘못된 데이터 길이: %d\n", len);
			}
			break;
		}
		case CS_DoorCheck:
		{
			CS_DoorOpenPacket* packet = reinterpret_cast<CS_DoorOpenPacket*>(buf);
			printf("\n");
			if (packet->num == 1) {
				g_player[0].SetDoor1Check(packet->value);
				g_player[1].SetDoor1Check(packet->value);
				{
					// 1번 플레이어 문상태 클라한테 뿌려주기
					len = sizeof(SC_DoorOpenPacket);
					SC_DoorOpenPacket* packet4 = new SC_DoorOpenPacket;
					packet4->type = SC_DoorCheck;
					packet4->num = 1;
					packet4->value = TRUE;
					for (int i = 0; i < MaxUser; ++i) { // 모든 클라이언트 순회
						if (g_player[i].AreUOnline() && g_player[i].GetSocket() != INVALID_SOCKET) {
							send(g_player[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
							send(g_player[i].GetSocket(), reinterpret_cast<char*>(packet4), len, 0);
						}
					}
					delete packet4;
				}
			}
			else if (packet->num == 2) {
				g_player[0].SetDoor2Check(packet->value);
				g_player[1].SetDoor2Check(packet->value);
				{
					// 2번 플레이어 문상태 클라한테 뿌려주기
					len = sizeof(SC_DoorOpenPacket);
					SC_DoorOpenPacket* packet5 = new SC_DoorOpenPacket;
					packet5->type = SC_DoorCheck;
					packet5->num = 2;
					packet5->value = TRUE;
					send(client_sock, reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(client_sock, reinterpret_cast<char*>(packet5), len, 0);
					delete packet5;
				}
			}
			break;
		}
		case CS_GOTKEY:
		{
			GotKeyPacket* packet = reinterpret_cast<GotKeyPacket*>(buf);
			SetCursorPosition(11);
			g_key.key[packet->key_num] = 1;
			g_key.keyNum = packet->key_num;
			printf("현재 키 %d개 획득\n", g_key.GetHowManyKey());
			{
				// 클라에 키 획득 현황 알려주기			
				BroadcastKeyState();
			}
			break;
		}
		case CS_GOTITEM:
		{
			GotItemPacket* packet = reinterpret_cast<GotItemPacket*>(buf);
			int item_num = packet->item_num;
			{
				// 클라에 아이템 획득 현황 알려주기				
				BroadcastItemState(item_num);
			}
			break;
		}
		case CS_Ending:
		{
			CS_GameOverPacket* packet = reinterpret_cast<CS_GameOverPacket*>(buf);
			{
				SC_GameOverPacket* packet6 = new SC_GameOverPacket;
				packet6->type = SC_Ending;
				packet6->value = packet->value;
				for (int i = 0; i < MaxUser; ++i) { // 모든 클라이언트 순회
					if (g_player[i].AreUOnline() && g_player[i].GetSocket() != INVALID_SOCKET) {
						send(g_player[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
						send(g_player[i].GetSocket(), reinterpret_cast<char*>(packet6), len, 0);
					}
				}
				delete packet6;
			}
			break;
		}

		// 소켓 닫기
		closesocket(client_sock);
		return 0;
		}
	}
}

DWORD WINAPI SendPacket(LPVOID IpParam)
{
	const int PACKET_SEND_INTERVAL_MS = 100;                         // 100ms (초당 10패킷)
	SC_AnotherPlayerCoordPacket* packet = new SC_AnotherPlayerCoordPacket; // 패킷 데이터 생성
	int len = sizeof(SC_AnotherPlayerCoordPacket);                      // 패킷 크기

	while (true) {
		// 패킷 전송 로직
		for (int i = 0; i < MaxUser; ++i) {
			if (g_player[i].AreUOnline() == TRUE) {
				if (g_player[i].GetSocket() != INVALID_SOCKET) {
					send(g_player[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(g_player[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
				}
			}
		}
		// 전송 간격 설정 (100ms 대기)
		std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_SEND_INTERVAL_MS));
	}
	delete packet; // 루프 종료 시 패킷 해제
}

//플레이어들끼리 충돌 처리
void PlayerCollision()
{

}

//문열릴 때 동시에 플레어가 도착 하였는지 확인
void CheckPlayersArrivalAtDoor()
{

}


void BroadcastKeyState()//키 상태 브로드캐스트 함수
{
	int len = sizeof(GotKeyPacket);

	// 키 상태를 모든 클라이언트에게 전송
	for (int i = 0; i < MaxUser; ++i) {
		if (g_player[i].AreUOnline() == TRUE) {
			SOCKET client_socket = g_player[i].GetSocket();
			if (client_socket != INVALID_SOCKET) {
				GotKeyPacket packet;
				packet.type = SC_GOTKEY;
				packet.HowManyKey = g_key.GetHowManyKey(); // 획득한 키 개수
				packet.key_num = g_key.keyNum;             // 마지막으로 획득한 키 번호

				// 패킷 전송
				send(client_socket, reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(client_socket, reinterpret_cast<char*>(&packet), len, 0);
			}
		}
	}
}
void BroadcastItemState(int item_num)//키 상태 브로드캐스트 함수
{
	int len = sizeof(GotItemPacket);

	// 키 상태를 모든 클라이언트에게 전송
	for (int i = 0; i < MaxUser; ++i) {
		if (g_player[i].AreUOnline() == TRUE) {
			SOCKET client_socket = g_player[i].GetSocket();
			if (client_socket != INVALID_SOCKET) {
				GotItemPacket packet;
				packet.type = SC_GOTITEM;
				packet.item_num = item_num;             // 마지막으로 획득한 아이템 번호

				// 패킷 전송
				send(client_socket, reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(client_socket, reinterpret_cast<char*>(&packet), len, 0);
			}
		}
	}
}