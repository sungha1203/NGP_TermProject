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

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// 소켓 생성
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

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	struct sockaddr_in clientaddr;
	HANDLE hThread;

	while (1)   ///////////////////////////////일단 클라1명만 해볼거임
	{
		// accept()
		int addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (struct sockaddr*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}

		++g_clientNum;	//클라이언트 접속 수						

		// 접속한 클라이언트 정보 출력
		char addr[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &clientaddr.sin_addr, addr, sizeof(addr));
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d, 1클라이언트 번호=%d\n",
			addr, ntohs(clientaddr.sin_port), g_clientNum);

		// 스레드 생성
		hThread = CreateThread(NULL, 0, ClientThread,
			(LPVOID)client_sock, 0, NULL);
		if (hThread == NULL) {
			closesocket(client_sock);
		}
		else {
			CloseHandle(hThread);
		}
	}

	// 소켓 닫기
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
		// 데이터 길이 수신
		retval = recv(client_sock, (char*)(&len), sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()"); // 오류 처리
			break;
		}
		else if (retval == 0) {
			printf("클라이언트가 연결을 종료했습니다.\n");
			break;
		}

		// 데이터 길이 검증
		if (len <= 0 || len > BUFSIZE) {
			printf("잘못된 데이터 길이: %d\n", len);
			continue; // 다음 루프로 넘어감
		}

		// 데이터 본문 수신
		memset(buf, 0, sizeof(buf));
		retval = recv(client_sock, buf, len, 0);
		if (retval <= 0) {
			printf("recv() 실패 또는 연결 종료.\n");
			break;
		}

		// 수신된 데이터를 구조체로 변환
		if (len == sizeof(PlayerCoordPacket)) {
			PlayerCoordPacket* packet = reinterpret_cast<PlayerCoordPacket*>(buf);
			printf("수신된 좌표: x=%.2f, y=%.2f, z=%.2f\n", packet->x, packet->y, packet->z);
		}
		else {
			printf("알 수 없는 패킷 또는 잘못된 데이터 길이: %d\n", len);
		}
	}

	// 소켓 닫기
	closesocket(client_sock);
	return 0;
}

//DWORD WINAPI SendPacket(LPVOID IpParam)
//{
//
//}

//플레이어들끼리 충돌 처리
void PlayerCollision()
{

}

//문열릴 때 동시에 플레어가 도착 하였는지 확인
void CheckPlayersArrivalAtDoor()
{

}