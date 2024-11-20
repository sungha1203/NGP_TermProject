#pragma once
#include "stdafx.h"
#include"Framework.h"

Framework& gGameFramework = Framework::getInstance(); // getInstance()를 통해 접근

int main(int argc, char** argv) {//--- 윈도우 출력하고 콜백함수 설정
	//if (!gGameFramework.network.IsConnect())return 0;
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