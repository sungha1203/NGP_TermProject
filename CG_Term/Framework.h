#pragma once
#define _CRT_SECURE_NO_WARNINGS //--- ���α׷� �� �տ� ������ ��
//#define STB_IMAGE_IMPLEMENTATION
#include "stdafx.h"
#include "camera.h"
#include "light.h"
//#include "LoadImage.h"
#include "OBB_check.h"
#include "maze.h"
#include "sphere.h"
#include <mmsystem.h>
#include <chrono>
#include <thread>
//#include"Common.h"
#include"Object.h"
#include"network.h"
//using namespace std;
//using namespace glm;
#define SOUND_FILE_NAME "walking_wav"
#pragma comment(lib,"winmm")
class GameObject;
class MAZE;
class SPHERE;

extern int My_Id;  // ���� ���° Ŭ������ �˶�� ������

class Framework
{
	Framework();
	~Framework();
public:
	static Framework* instance; // ���� �ν��Ͻ� ������

	static Framework& getInstance();
	void make_vertexShaders();
	void make_fragmentShaders();
	static GLvoid drawScene(GLvoid);
	//GLvoid Reshape(int w, int h);
	static GLvoid KeyBoardFunc(unsigned char key, int x, int y);
	static GLvoid Motion(int x, int y);
	GLuint make_shaderProgram();
	static GLvoid Timer(int value);
	static GLvoid KeyBoardUpFunc(unsigned char key, int x, int y);
	void BuildObjects();
	//-----------�ʿ��� ���� ����----------------
	GLuint shaderProgramID; //--- ���̴� ���α׷� �̸�
	GLuint vertexShader;    //--- ���ؽ� ���̴� ��ü
	GLuint fragmentShader;
	GLint result;
	GLchar errorLog[512];
	//-------------------------------------------

	Light light;
	Light light_minimap;
	vec2 oldmouse;
	vec2 newmouse;
	vec3 camerapos{ -12.0f,0.2f,-9.5f };
	vec3 playerpos{ -12.0f,0.2f,-9.5f };
public:
	unsigned int texture[11];
public:
	vector <MAZE> maze;
	vector <SPHERE> sphere;
	GameObject** m_ppObject;
	int			m_nGameObjects = 0;
public:
	vector <glm::vec3> D_vertex;
	vector <glm::vec3> D_normal;
	vector <glm::vec2> D_vt;

	vector <glm::vec3> S_vertex;
	vector <glm::vec3> S_normal;
	vector <glm::vec2> S_vt;

	
	CAMERA camera;
	CAMERA minimap_camera;

public:
	float g_width = 1000;
	float g_height = 1000;
	bool button_m = FALSE;
	bool creativemode = FALSE;
	bool light_button = TRUE;
	bool isWalking = GL_FALSE; // ��å �� ���θ� ��Ÿ���� ���� �߰�
	vec3 CMPOS;
	//int item_navi = 0;
	//int item_where = 0;
	//int item_Key_where = 0;
	int hintIndex = -1;
	bool ũ������Ƽ���� = false;

	int howManyKey = 0;
	int Mode = 0;  //���� ���� �� ����
public:
	void objectcollision();//�浹
	bool maze_collision();
	bool door_collision();
public:
	Network network;
};