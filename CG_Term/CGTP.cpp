#pragma once
#include "stdafx.h"
#include "Framework.h"

//다른 클라이언트 플레이어 정보 구조체
struct Player
{
	int id;
	float x;
	float y;
	float z;
};

Player g_player[2];
std::mutex player_mutex;

Framework& gGameFramework = Framework::getInstance(); // getInstance()를 통해 접근
DWORD WINAPI RecvThread(LPVOID lpParam);

int main(int argc, char** argv) {//--- 윈도우 출력하고 콜백함수 설정
	if (!gGameFramework.network.IsConnect())return 0;
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(gGameFramework.g_width, gGameFramework.g_height); // 윈도우의 크기 지정
	glutCreateWindow("OPENGL"); // 윈도우 생성
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << endl;

	HANDLE hThread = CreateThread(NULL, 0, RecvThread, 0, 0, 0);
	if (hThread == NULL) {
		closesocket(gGameFramework.network.getsock());
	}
	else {
		CloseHandle(hThread);
	}

	gGameFramework.make_vertexShaders(); //--- 버텍스 세이더 만들기
	gGameFramework.make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	gGameFramework.shaderProgramID = gGameFramework.make_shaderProgram();
	gGameFramework.BuildObjects();

	glutTimerFunc(17, gGameFramework.Timer, CamMove);
	gGameFramework.light_minimap.lightpos = { 0,28,0 };
	glutTimerFunc(17, gGameFramework.Timer, 1);
	glutTimerFunc(17, gGameFramework.Timer, 2);
	glutTimerFunc(34, gGameFramework.Timer, 3);
	glutTimerFunc(34, gGameFramework.Timer, 4);
	glutTimerFunc(17, gGameFramework.Timer, 5);
	glutPassiveMotionFunc(gGameFramework.Motion);
	glutKeyboardFunc(gGameFramework.KeyBoardFunc);
	glutTimerFunc(17, gGameFramework.Timer, CamMove);
	glutKeyboardUpFunc(gGameFramework.KeyBoardUpFunc);
	glutTimerFunc(20, gGameFramework.Timer, Live);
	glutDisplayFunc(gGameFramework.drawScene); //--- 출력 콜백 함수
	//glutReshapeFunc(Reshape);
	glutMainLoop(); // 이벤트 처리 시작
}

DWORD WINAPI RecvThread(LPVOID lpParam)
{
	while (1) {
		int len = 0;
		char buf[BUFSIZE];
		recv(gGameFramework.network.getsock(), (char*)&len, sizeof(int), MSG_WAITALL);
		recv(gGameFramework.network.getsock(), buf, len, MSG_WAITALL);

		switch (buf[0]) {
		case SC_EnterId:
		{
			SC_EnterIdPacket* packet = reinterpret_cast<SC_EnterIdPacket*>(buf);
			g_player[packet->id - 1].id = packet->id;
			My_Id = packet->id;
			gGameFramework.id = packet->id;
			printf("%d번 플레이어님이 입장하셨습니다!\n", packet->id);
			printf("--------------------------------------------------\n");
			break;
		}
		case SC_AnotherCoord:
		{
			//std::lock_guard<std::mutex> lock(player_mutex);
			SC_AnotherPlayerCoordPacket* packet = reinterpret_cast<SC_AnotherPlayerCoordPacket*>(buf);
			gGameFramework.id = packet->id;
			gGameFramework.P2_pos.x = packet->x;
			gGameFramework.P2_pos.y = packet->y;
			gGameFramework.P2_pos.z = packet->z;
			gGameFramework.At = packet->cameraAt;
			//printf("%d번 플레이어님의 좌표 : x = %.2f, y = %.2f, z = %.2f\n", packet->id, packet->x, packet->y, packet->z);
			//cout << "다른 플레이어 방향: " << gGameFramework.At.x << ", " << gGameFramework.At.y << ", " << gGameFramework.At.z << endl;
			gGameFramework.sock_check = true;
			break;
		}
		case SC_DoorCheck:
		{
			SC_DoorOpenPacket* packet = reinterpret_cast<SC_DoorOpenPacket*>(buf);
			if (packet->num == 1) {
				g_door1 = packet->value;
				printf("문 열게용~\n");
			}
			else if (packet->num == 2) {
				g_door2 = packet->value;
			}
			break;
		}
		case SC_GOTKEY:
		{
			GotKeyPacket* packet = reinterpret_cast<GotKeyPacket*>(buf);
			gGameFramework.howManyKey = packet->HowManyKey;
			gGameFramework.m_ppObject[packet->key_num + 7]->exist = false;
			PlaySound(TEXT("keysound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			break;
		}
		case SC_Ending:
		{
			SC_GameOverPacket* packet = reinterpret_cast<SC_GameOverPacket*>(buf);
			g_ending = packet->value;
			printf("GAME CLEAR!!!!!");
		}
		}
	}
}
