#pragma once
#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
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

extern int My_Id; 
extern bool g_door1;
extern bool g_door2;

class Framework
{
	Framework();
	~Framework();
public:
	static Framework* instance; // 전역 인스턴스 포인터
	static Framework& getInstance();

	void make_vertexShaders();
	void make_fragmentShaders();
	GLuint make_shaderProgram();

	static GLvoid drawScene(GLvoid);
	//GLvoid Reshape(int w, int h);
	static GLvoid KeyBoardFunc(unsigned char key, int x, int y);
	static GLvoid Motion(int x, int y);
	static GLvoid Timer(int value);
	static GLvoid KeyBoardUpFunc(unsigned char key, int x, int y);
	void BuildObjects();
	//-----------필요한 변수 선언----------------
	GLuint shaderProgramID; //--- 세이더 프로그램 이름
	GLuint vertexShader;    //--- 버텍스 세이더 객체
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
	unsigned int texture[13];
public:
	vector <MAZE> maze;
	vector <SPHERE> sphere;
	GameObject** m_ppObject;

	///////////////////////////////////////////////////////////////////////////////
	vec3 P2_pos{}; //다른 클라 위치 정보
	vec3 At{}; //다른 클라 방향 벡터
	int id;



	int         m_nGameObjects = 0;
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
	bool isWalking = GL_FALSE; // 산책 중 여부를 나타내는 변수 추가
	vec3 CMPOS;
	//int item_navi = 0;
	//int item_where = 0;
	//int item_Key_where = 0;
	int hintIndex = -1;
	bool create_mode = false;

	int howManyKey = 0;
	int Mode = 0;  //게임 시작 및 엔딩
	bool sock_check = false; //다른 클라이언트가 접속했는지 여부
public:
	void objectcollision();//충돌
	bool maze_collision();
	bool door_collision();
	bool player_collision();
public:
	Network network;
};