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
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) 
        return false;

    // ���� ����
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        err_quit("socket()");
        return false;
    }

    // ���� ����
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
    // ���� ��ȿ�� �˻�
    if (sock == INVALID_SOCKET) {
        printf("������ ��ȿ���� �ʽ��ϴ�. �����͸� ���� �� �����ϴ�.\n");
        return;
    }

    int result = send(sock, reinterpret_cast<char*>(&size), sizeof(int), 0);
    if (result == SOCKET_ERROR) {
        printf("send() ������ ���� ���� ����1: %d\n", WSAGetLastError());
    }
    result = send(sock, packet, size, 0);
    if (result == SOCKET_ERROR) {
        printf("send() ������ ���� ����2: %d\n", WSAGetLastError());
    }
    //printf("��Ŷ ���� ����: %d ����Ʈ\n", size);      //���������� Ȯ���غ����� ������
}