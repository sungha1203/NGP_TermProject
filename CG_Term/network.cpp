#include "stdafx.h"
#include "network.h"
#include "error.h"

char* server_ip = (char*)"127.0.0.1";

Network::Network()
{
	sock = INVALID_SOCKET;
}

Network::~Network()
{
	closesocket(sock);

}

bool Network::IsConnect()
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
        return false;

    // 소켓 생성
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        err_quit("socket()");
        return false;
    }

    // 서버 연결
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, server_ip, &serveraddr.sin_addr);
    serveraddr.sin_port = htons(SERVERPORT);
    int retval = connect(sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) {
        err_quit("connect()");
        return false;
    }
    return true;
}

void Network::SendPacket()
{

}