#include "error.h"
#include "protocol.h"
#include "PlayerInfo.h"
#include <vector>
#include <thread>
#include "chrono"
#include <mutex>

std::mutex g_playerMutex;

DWORD WINAPI ClientThread(LPVOID socket);
DWORD WINAPI SendPacket(LPVOID IpParam);

std::vector<PlayerInfo> g_player(MaxUser);

int g_clientNum{};

void PlayerCollision();
void CheckPlayersArrivalAtDoor();

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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
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

	HANDLE SendThread;
	SendThread = CreateThread(NULL, 0, SendPacket, 0, 0, 0);
	if (SendThread == NULL) {
		closesocket(listen_sock);
	}
	else {
		CloseHandle(SendThread);
	}

	// ������ ��ſ� ����� ����
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
			g_player[g_clientNum].SetSocket(client_socket);     // ���� ����
			g_player[g_clientNum].SetOnline();                  // �¶��� Ȱ��ȭ
			g_player[g_clientNum].SetId(g_clientNum + 1);       // ID �ο�

			// ������ Ŭ���̾�Ʈ ���� ���
			char addr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
			printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ� = %s, ��Ʈ ��ȣ = %d, Ŭ���̾�Ʈ ��ȣ = %d�� �÷��̾�\n",
				addr, ntohs(clientaddr.sin_port), g_player[g_clientNum].GetId());

			// Ŭ���̾�Ʈ���� ���� �˸� ��Ŷ ����
			{
				int len = sizeof(SC_EnterIdPacket);
				SC_EnterIdPacket* packet = new SC_EnterIdPacket;
				packet->type = SC_EnterId;
				packet->id = g_clientNum + 1;
				send(client_socket, reinterpret_cast<char*>(&len), sizeof(int), 0);
				send(client_socket, reinterpret_cast<char*>(packet), len, 0);
				delete packet;
			}

			// Ŭ���̾�Ʈ�� ó���� ������ ����
			HANDLE hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)client_socket, 0, NULL);
			if (hThread == NULL) {
				closesocket(client_socket);
			}
			else {
				CloseHandle(hThread);
			}

			// Ŭ���̾�Ʈ �� ����
			++g_clientNum;
			printf("���� ���� Ŭ���̾�Ʈ �� : %d / %d\n", g_clientNum, MaxUser);
		}
		else {
			// accept ���� �� ���� �޽��� ���
			err_display("accept() ����: Ŭ���̾�Ʈ ������ ��ȿ���� �ʽ��ϴ�.");
		}
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

DWORD WINAPI ClientThread(LPVOID socket)
{
	int retval;
	//PlayerInfo* sock = reinterpret_cast<PlayerInfo*>(socket);
	//SOCKET client_sock = sock->GetSocket();
	SOCKET client_sock = reinterpret_cast<SOCKET>(socket);

	int len;
	char buf[BUFSIZE + 1];

	while (1) {
		// ������ ���� ����
		retval = recv(client_sock, (char*)(&len), sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()"); // ���� ó��
			break;
		}
		else if (retval == 0) {
			printf("Ŭ���̾�Ʈ�� ������ �����߽��ϴ�.\n");
			break;
		}

		// ������ ���� ����
		if (len <= 0 || len > BUFSIZE) {
			printf("�߸��� ������ ����: %d\n", len);
			continue; // ���� ������ �Ѿ
		}

		// ������ ���� ����
		memset(buf, 0, sizeof(buf));
		retval = recv(client_sock, buf, len, 0);
		if (retval <= 0) {
			printf("recv() ���� �Ǵ� ���� ����.\n");
			break;
		}

		// ���ŵ� �����͸� ����ü�� ��ȯ
		if (len == sizeof(PlayerCoordPacket)) {
			PlayerCoordPacket* packet = reinterpret_cast<PlayerCoordPacket*>(buf);
			if (packet->id == 1) {   // 1�� �÷��̾� ��ǥ �ޱ�
				SetCursorPosition(7);
				printf("%d�� �÷��̾��� ��ǥ: x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->x, packet->y, packet->z);
				g_player[packet->id - 1].SetCoord(packet->x, packet->y, packet->z);
				{
					// 2�� ��ǥ 1������ �����ֱ�
					len = sizeof(SC_AnotherPlayerCoordPacket);
					SC_AnotherPlayerCoordPacket* packet2 = new SC_AnotherPlayerCoordPacket;
					packet2->type = SC_AnotherCoord;
					packet2->id = g_player[1].GetId();
					packet2->x = g_player[1].GetCoordX();
					packet2->y = g_player[1].GetCoordY();
					packet2->z = g_player[1].GetCoordZ();

					send(client_sock, reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(client_sock, reinterpret_cast<char*>(packet2), len, 0);
					delete packet2;
				}
			}
			else					// 2�� �÷��̾� ��ǥ �ޱ�
			{
				SetCursorPosition(9);
				printf("%d�� �÷��̾��� ��ǥ: x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->x, packet->y, packet->z);
				g_player[packet->id - 1].SetCoord(packet->x, packet->y, packet->z);
				{
					// 1�� ��ǥ 2������ �����ֱ�
					len = sizeof(SC_AnotherPlayerCoordPacket);
					SC_AnotherPlayerCoordPacket* packet3 = new SC_AnotherPlayerCoordPacket;
					packet3->type = SC_AnotherCoord;
					packet3->id = g_player[0].GetId();
					packet3->x = g_player[0].GetCoordX();
					packet3->y = g_player[0].GetCoordY();
					packet3->z = g_player[0].GetCoordZ();

					send(client_sock, reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(client_sock, reinterpret_cast<char*>(packet3), len, 0);
					delete packet3;
				}
			}
		}
		else {
			printf("�� �� ���� ��Ŷ �Ǵ� �߸��� ������ ����: %d\n", len);
		}
	}

	// ���� �ݱ�
	closesocket(client_sock);
	return 0;
}

DWORD WINAPI SendPacket(LPVOID IpParam)
{
	const int PACKET_SEND_INTERVAL_MS = 100;                         // 100ms (�ʴ� 10��Ŷ)
	SC_AnotherPlayerCoordPacket* packet = new SC_AnotherPlayerCoordPacket; // ��Ŷ ������ ����
	int len = sizeof(SC_AnotherPlayerCoordPacket);                      // ��Ŷ ũ��

	while (true) {
		// ��Ŷ ���� ����
		for (int i = 0; i < MaxUser; ++i) {
			if (g_player[i].AreUOnline() == TRUE) {
				if (g_player[i].GetSocket() != INVALID_SOCKET) {
					send(g_player[i].GetSocket(), reinterpret_cast<char*>(&len), sizeof(int), 0);
					send(g_player[i].GetSocket(), reinterpret_cast<char*>(packet), len, 0);
				}
			}
		}

		// ���� ���� ���� (100ms ���)
		std::this_thread::sleep_for(std::chrono::milliseconds(PACKET_SEND_INTERVAL_MS));
	}
	delete packet; // ���� ���� �� ��Ŷ ����
}

//�÷��̾�鳢�� �浹 ó��
void PlayerCollision()
{

}

//������ �� ���ÿ� �÷�� ���� �Ͽ����� Ȯ��
void CheckPlayersArrivalAtDoor()
{

}