#include <windows.h>
#include "error.h"
#include "protocol.h"

DWORD WINAPI ClientThread(LPVOID socket);
DWORD WINAPI SendPacket(LPVOID IpParam);

void PlayerCollision();
void CheckPlayersArrivalAtDoor();

int main() 
{
	int retval;
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//���� ����
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	//bind()	
	struct sockaddr_in serveraddr;
	ZeroMemory(&serveraddr, sizeof 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	int addrlen;
	HANDLE hThread;

	

	WSACleanup();
	return 0;
}

DWORD WINAPI ClientThread(LPVOID socket)
{

}

DWORD WINAPI SendPacket(LPVOID IpParam)
{

}

//�÷��̾�鳢�� �浹 ó��
void PlayerCollision()
{

}

//������ �� ���ÿ� �÷�� ���� �Ͽ����� Ȯ��
void CheckPlayersArrivalAtDoor()
{

}