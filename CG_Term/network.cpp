#include "stdafx.h"
#include "network.h"
#include "error.h"

char* server_ip = (char*)"172.30.1.53";

Network::Network()
{
	sock = INVALID_SOCKET;
}

Network::~Network()
{
	closesocket(sock);
    WSACleanup();
}

SOCKET Network::getsock()
{
    return sock;
}

bool Network::IsConnect()
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
        return false;

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
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
        closesocket(sock);
        return false;
    }
    return true;
}

void Network::SendPacket(char* packet, int size)
{
    // 소켓 유효성 검사
    if (sock == INVALID_SOCKET) {
        printf("소켓이 유효하지 않습니다. 데이터를 보낼 수 없습니다.\n");
        return;
    }

    int result = send(sock, reinterpret_cast<char*>(&size), sizeof(int), 0);
    if (result == SOCKET_ERROR) {
        printf("send() 데이터 길이 전송 실패1: %d\n", WSAGetLastError());
    }
    result = send(sock, packet, size, 0);
    if (result == SOCKET_ERROR) {
        printf("send() 데이터 전송 실패2: %d\n", WSAGetLastError());
    }
    //printf("패킷 전송 성공: %d 바이트\n", size);      //잘전해지나 확인해보려고 쓴거임
}