#include "error.h"
#include "protocol.h"

DWORD WINAPI ClientThread(LPVOID socket);
//DWORD WINAPI SendPacket(LPVOID IpParam);

int g_clientNum{};

void PlayerCollision();
void CheckPlayersArrivalAtDoor();

int main()
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// ���� ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	HANDLE hThread;

	while (1)   ///////////////////////////////�ϴ� Ŭ��1�� �غ�����
	{
		// accept()
		int addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		++g_clientNum;	//Ŭ���̾�Ʈ ���� ��						

		// ������ Ŭ���̾�Ʈ ���� ���
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d, 1Ŭ���̾�Ʈ ��ȣ=%d\n",
			addr, ntohs(clientaddr.sin_port), g_clientNum);

		// ������ ����
		hThread = CreateThread(NULL, 0, ClientThread,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}
	}

	// ���� �ݱ�
	closesocket(listen_sock);

	WSACleanup();
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	int retval;
	SOCKET client_sock = (SOCKET)arg;

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
			printf("���ŵ� ��ǥ: x=%.2f, y=%.2f, z=%.2f\n", packet->x, packet->y, packet->z);
		}
		else {
			printf("�� �� ���� ��Ŷ �Ǵ� �߸��� ������ ����: %d\n", len);
		}
	}

	// ���� �ݱ�
	closesocket(client_sock);
	return 0;
}

//DWORD WINAPI SendPacket(LPVOID IpParam)
//{
//
//}

//�÷��̾�鳢�� �浹 ó��
void PlayerCollision()
{

}

//������ �� ���ÿ� �÷�� ���� �Ͽ����� Ȯ��
void CheckPlayersArrivalAtDoor()
{

}