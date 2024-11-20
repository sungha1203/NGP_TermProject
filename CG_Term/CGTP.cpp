#pragma once
#include "stdafx.h"
#include"Framework.h"

Framework& gGameFramework = Framework::getInstance(); // getInstance()�� ���� ����

int main(int argc, char** argv) {//--- ������ ����ϰ� �ݹ��Լ� ����
	//if (!gGameFramework.network.IsConnect())return 0;
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(gGameFramework.g_width, gGameFramework.g_height); // �������� ũ�� ����
	glutCreateWindow("OPENGL"); // ������ ����
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << endl;
	gGameFramework.make_vertexShaders(); //--- ���ؽ� ���̴� �����
	gGameFramework.make_fragmentShaders(); //--- �����׸�Ʈ ���̴� �����
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
	glutDisplayFunc(gGameFramework.drawScene); //--- ��� �ݹ� �Լ�
	//glutReshapeFunc(Reshape);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}