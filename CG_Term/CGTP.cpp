#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS //--- 프로그램 맨 앞에 선언할 것
#define STB_IMAGE_IMPLEMENTATION
#include "camera.h"
#include "light.h"
#include "LoadImage.h"
#include "OBB_check.h"
#include "maze.h"
#include "sphere.h"
#include "Player.h"
#include "Player_check.h"
#include "network.h"
#include <mmsystem.h>

#include <chrono>
#include <thread>
using namespace std;
using namespace glm;
#define SOUND_FILE_NAME "walking_wav"
#pragma comment(lib,"winmm")

void make_vertexShaders();
void make_fragmentShaders();
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid KeyBoardFunc(unsigned char key, int x, int y);
GLvoid Motion(int x, int y);
GLuint make_shaderProgram();
GLvoid Timer(int value);
GLvoid KeyBoardUpFunc(unsigned char key, int x, int y);

//-----------필요한 변수 선언----------------
GLuint shaderProgramID; //--- 세이더 프로그램 이름
GLuint vertexShader;    //--- 버텍스 세이더 객체
GLuint fragmentShader;
GLint result;
GLchar errorLog[512];
//-------------------------------------------

//--------------네트워크 통신----------------
Network network;
//-------------------------------------------

Light light;
Light light_minimap;
vec2 oldmouse;
vec2 newmouse;
vec3 camerapos{ -12.0f,0.2f,-9.5f };


vector <glm::vec3> D_vertex;
vector <glm::vec3> D_normal;
vector <glm::vec2> D_vt;

vector <glm::vec3> S_vertex;
vector <glm::vec3> S_normal;
vector <glm::vec2> S_vt;

vector <glm::vec3> plane_vertex;
vector <glm::vec3> plane_normal;
vector <glm::vec2> plane_vt;

vector <glm::vec3> door_vertex;
vector <glm::vec3> door_normal;
vector <glm::vec2> door_vt;

vector <glm::vec3> item_vertex;
vector <glm::vec3> item_normal;
vector <glm::vec2> item_vt;

vector <glm::vec3> key_vertex;
vector <glm::vec3> key_normal;
vector <glm::vec2> key_vt;

vector <glm::vec3> ghost_vertex;
vector <glm::vec3> ghost_normal;
vector <glm::vec2> ghost_vt;

vector <glm::vec3> mode_vertex;
vector <glm::vec3> mode_normal;
vector <glm::vec2> mode_vt;

vector <glm::vec3> xyz_vertex;
vector <glm::vec3> hint_vertex;

vector <glm::vec3> Player_vertex;
vector <glm::vec3> Player_normal;
vector <glm::vec2> Player_vt;

vector <glm::vec3> Player_check_vertex;
vector <glm::vec3> Player_check_normal;
vector <glm::vec2> Player_check_vt;

//----------------전역 변수------------------
float g_width = 1000;
float g_height = 1000;
bool button_m = FALSE;
bool creativemode = FALSE;
bool light_button = TRUE;
bool isWalking = GL_FALSE; // 산책 중 여부를 나타내는 변수 추가
unsigned int texture[11];

vec3 CMPOS;
int item_navi = 0;
int item_where = 0;
int item_Key_where = 0;
bool 크리에이티브모드 = false;

int howManyKey = 0;
int Mode = 0;  //게임 시작 및 엔딩

//-------------------------------------------


struct make_plane {

	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 2.0f,1.0f,0.5f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	make_plane(int num) {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;

		if (num == 0) {
			move_pos = { -12.0f,0.0f,-9.4f };
		}
		else if (num == 1) {
			move_pos = { -12.0f,0.0f,-6.46f };
		}
		SetVBO();
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * plane_vertex.size(), &plane_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * plane_normal.size(), &plane_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * plane_vt.size(), &plane_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));


		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);

		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};
struct make_door {
	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 1.0f,0.5f,0.324f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	int m_IsOpen = 0;
	int m_OpenN = 0;
	make_door(int num) {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;

		if (num == 1) {
			move_pos = { -9.8f,0.0f,-9.08f };
		}
		else if (num == 2) {
			rotateAngle[1] = 180.0f;
			move_pos = { -9.8f,0.0f,-9.728f };
		}
		if (num == 3) {
			move_pos = { -9.96f,0.0f,-6.1f };
		}
		else if (num == 4) {
			rotateAngle[1] = 180.0f;
			move_pos = { -9.96f,0.0f,-6.748f };
		}
		SetVBO();
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * door_vertex.size(), &door_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * door_normal.size(), &door_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * door_vt.size(), &door_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));


		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);

		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};
struct make_item {
	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 3.f,3.f,3.f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	bool m_direction = true;
	int m_direcN = 0;
	bool exist = true;
	int m_ability = -1;

	make_item(int num) {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 0.0f;

		if (num == 0) {
			move_pos = { -9.05f,0.1f,-9.4f };
			m_ability = 1;
		}
		else if (num == 1) {
			move_pos = { -0.5f,0.1f,-4.5f };
			m_ability = 5;
		}
		else if (num == 2) {
			move_pos = { -2.5f,0.1f,-0.5f };
			m_ability = 2;
		}
		else if (num == 3) {
			move_pos = { -4.5f,0.1f,-2.5f };
			m_ability = 1;
		}
		else if (num == 4) {
			move_pos = { -8.5f,0.1f,-7.5f };
			m_ability = 3;
		}
		else if (num == 5) {
			move_pos = { -6.5f,0.1f,-2.5f };
			m_ability = 1;
		}
		else if (num == 6) {
			move_pos = { -1.5f,0.1f,3.5f };
			m_ability = 5;
		}
		else if (num == 7) {
			move_pos = { 1.5f,0.1f,6.5f };
			m_ability = 5;
		}
		else if (num == 8) {
			move_pos = { 3.5f,0.1f,8.5f };
			m_ability = 1;
		}
		else if (num == 9) {
			move_pos = { 6.5f,0.1f,6.5f };
			m_ability = 2;
		}
		else if (num == 10) {
			move_pos = { 5.5f,0.1f,4.5f };
			m_ability = 1;
		}
		else if (num == 11) {
			move_pos = { 9.5f,0.1f,5.5f };
			m_ability = 1;
		}
		else if (num == 12) {
			move_pos = { 6.5f,0.1f,-1.2f };
			m_ability = 5;
		}
		else if (num == 13) {
			move_pos = { 4.5f,0.1f,-3.5f };
			m_ability = 2;
		}
		else if (num == 14) {
			move_pos = { 2.0f,0.1f,-9.5f };
			m_ability = 1;
		}
		else if (num == 15) {
			move_pos = { 7.5f,0.1f,-9.5f };
			m_ability = 5;
		}
		else if (num == 16) {
			move_pos = { -9.5f,0.1f,6.5f };
			m_ability = 2;
		}
		else if (num == 17) {
			move_pos = { -8.5f,0.1f,0.5f };
			m_ability = 1;
		}
		else if (num == 18) {
			move_pos = { -4.5f,0.1f,5.5f };
			m_ability = 3;
		}
		else if (num == 19) {
			move_pos = { -13.8f,0.1f,-9.4f };
			m_ability = 0;
		}

		SetVBO();
	}

	void ability() {
		if (m_ability == 0) {  //다 보여주기
			item_navi = 1;
			item_where = 1;
			item_Key_where = 1;
			크리에이티브모드 = true;
		}
		else if (m_ability == 1) {  //네비게이션
			item_navi = 1;
			thread timerThread(&make_item::startTimer1, this);
			timerThread.detach();
		}
		else if (m_ability == 2) {  //아이템 힌트
			item_where = 1;
			thread timerThread(&make_item::startTimer2, this);
			timerThread.detach();
		}
		else if (m_ability == 3) {  //도르마무
			camerapos.x = -12.0f;
			camerapos.z = -9.5f;
		}
		else if (m_ability == 5) {  //열쇠 힌트
			item_Key_where = 1;
		}
	}

	void startTimer1() {
		this_thread::sleep_for(std::chrono::seconds(10));
		m_ability = -1;
		item_navi = 0;
	}

	void startTimer2() {
		this_thread::sleep_for(std::chrono::seconds(10));
		m_ability = -1;
		item_where = 0;
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * item_vertex.size(), &item_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * item_normal.size(), &item_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * item_vt.size(), &item_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색

		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);

		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glDrawArrays(GL_TRIANGLES, 0, item_vertex.size());
	}
};
struct make_key {
	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 0.05f,0.05f,0.05f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	bool m_direction = true;
	int m_direcN = 0;
	bool exist = true;

	make_key(int num) {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;

		if (num == 0) {
			move_pos = { -2.5, 0.1, -9.4 };
		}
		else if (num == 1) {
			move_pos = { 9.5f,0.1f,-8.5f };
		}
		else if (num == 2) {
			move_pos = { -8.5f,0.1f,4.5f };
		}

		SetVBO();
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * key_vertex.size(), &key_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * key_normal.size(), &key_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * key_vt.size(), &key_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색

		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);

		glBindTexture(GL_TEXTURE_2D, texture[5]);

		glDrawArrays(GL_TRIANGLES, 0, key_vertex.size());
	}
};
struct make_bottom {

	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 100.0f,1.0f,100.0f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	make_bottom() {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;

		move_pos = { 0.0f,-8.0f,0.0f };
		SetVBO();
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * plane_vertex.size(), &plane_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * plane_normal.size(), &plane_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * plane_vt.size(), &plane_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));


		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);

		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};
struct make_hint {
	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	vec3 m_color;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 1.0f,1.0f,1.0f };
	vec3 move_pos = { 0.0f,0.0f,0.0f };
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	int m_ability;  //0. 좋은거 다  1.길 힌트  2.아이템 위치  3.열쇠 위치

	make_hint() {
		m_color.r = 1.0f;
		m_color.g = 1.0f;
		m_color.b = 1.0f;

		hint_vertex.emplace_back(-10, 0.02, -9.4);
		hint_vertex.emplace_back(-2.5, 0.02, -9.4);
		hint_vertex.emplace_back(-2.5, 0.02, -3.5);
		hint_vertex.emplace_back(-3.5, 0.02, -3.5);
		hint_vertex.emplace_back(-3.5, 0.02, -8.5);
		hint_vertex.emplace_back(-7.5, 0.02, -8.5);
		hint_vertex.emplace_back(-7.5, 0.02, -7.5);
		hint_vertex.emplace_back(-4.5, 0.02, -7.5);
		hint_vertex.emplace_back(-4.5, 0.02, -6.5);
		hint_vertex.emplace_back(-5.5, 0.02, -6.5);
		hint_vertex.emplace_back(-5.5, 0.02, -4.5);
		hint_vertex.emplace_back(-6.5, 0.02, -4.5);
		hint_vertex.emplace_back(-7.5, 0.02, -2.5);
		hint_vertex.emplace_back(-6.5, 0.02, -0.5);
		hint_vertex.emplace_back(-4.5, 0.02, -0.5);
		hint_vertex.emplace_back(-4.5, 0.02, 2.5);
		hint_vertex.emplace_back(-5.5, 0.02, 2.5);
		hint_vertex.emplace_back(-5.5, 0.02, 3.5);
		hint_vertex.emplace_back(-3.5, 0.02, 3.5);
		hint_vertex.emplace_back(-1.5, 0.02, 2.5);
		hint_vertex.emplace_back(-0.5, 0.02, 2.5);
		hint_vertex.emplace_back(2.5, 0.02, 1.15);
		hint_vertex.emplace_back(1.9, 0.02, -1.0);
		hint_vertex.emplace_back(3.3, 0.02, -4.0);
		hint_vertex.emplace_back(2.5, 0.02, -5.0);
		hint_vertex.emplace_back(2.5, 0.02, -8.5);
		hint_vertex.emplace_back(-1.5, 0.02, -8.5);
		hint_vertex.emplace_back(-1.5, 0.02, -9.5);
		hint_vertex.emplace_back(6.5, 0.02, -9.5);
		hint_vertex.emplace_back(6.5, 0.02, -7.5);
		hint_vertex.emplace_back(9.5, 0.02, -7.5);
		hint_vertex.emplace_back(9.5, 0.02, 3.5);
		hint_vertex.emplace_back(8.5, 0.02, 3.5);
		hint_vertex.emplace_back(8.5, 0.02, 2.0);
		hint_vertex.emplace_back(6.5, 0.02, 0.0);
		hint_vertex.emplace_back(6.5, 0.02, 1.5);
		hint_vertex.emplace_back(8.5, 0.02, 5.5);
		hint_vertex.emplace_back(5.5, 0.02, 5.5);
		hint_vertex.emplace_back(5.5, 0.02, 7.5);
		hint_vertex.emplace_back(2.5, 0.02, 7.5);
		hint_vertex.emplace_back(2.5, 0.02, 5.5);
		hint_vertex.emplace_back(-1.5, 0.02, 5.5);
		hint_vertex.emplace_back(-1.5, 0.02, 4.5);
		hint_vertex.emplace_back(-6.5, 0.02, 4.5);
		hint_vertex.emplace_back(-6.5, 0.02, 1.5);
		hint_vertex.emplace_back(-5.5, 0.02, 1.5);
		hint_vertex.emplace_back(-5.5, 0.02, 0.5);
		hint_vertex.emplace_back(-7.5, 0.02, 0.5);
		hint_vertex.emplace_back(-7.5, 0.02, 3.5);
		hint_vertex.emplace_back(-9.5, 0.02, 3.5);
		hint_vertex.emplace_back(-9.5, 0.02, -3.5);
		hint_vertex.emplace_back(-8.5, 0.02, -3.5);
		hint_vertex.emplace_back(-8.5, 0.02, -4.5);
		hint_vertex.emplace_back(-9.5, 0.02, -4.5);
		hint_vertex.emplace_back(-9.5, 0.02, -5.5);
		hint_vertex.emplace_back(-7.5, 0.02, -5.5);
		hint_vertex.emplace_back(-7.5, 0.02, -6.5);
		hint_vertex.emplace_back(-10.0, 0.02, -6.5);
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
	}

	void draw() {

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * hint_vertex.size(), &hint_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

		int vColorLocation = glGetUniformLocation(shaderProgramID, "objectColor");

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 0);

		glLineWidth(1.0f);
		glUniform3f(vColorLocation, m_color.r, m_color.g, m_color.b);
		glDrawArrays(GL_LINE_STRIP, 0, hint_vertex.size());

	}
};
struct make_ghost {
	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 0.05f,0.05f,0.05f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	bool m_direction = true;
	int m_direcN = 0;
	float speed;
	int direction = 1;
	make_ghost(int num) {
		random_device rd;
		default_random_engine dre(rd());
		uniform_real_distribution<float> urd(0.01, 0.04);
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;
		for (int i = 0; i < 10; ++i)
		{
			if (num == i) {
				move_pos = { -10.0 + 2 * (i + 1) - 0.5f, 0.2, -11.0 };
			}
		}

		for (int i = 10; i < 20; ++i)
		{
			if (num == i) {
				move_pos = { 11.0f, 0.2, -10.0 + 2 * ((i - 10) + 1) - 0.5 };
				rotateAngle[1] = 90;
			}
		}
		speed = urd(dre);

		SetVBO();
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void Move(int movetype)
	{
		if (movetype == 1) {
			move_pos.z += direction * speed;
			if (move_pos.z > 11.0 || move_pos.z < -11.0)
			{
				direction *= -1;
				rotateAngle[1] += 180 * direction;
			}
		}
		else
		{
			move_pos.x += direction * speed;
			if (move_pos.x > 11.0 || move_pos.x < -11.0)
			{
				direction *= -1;
				rotateAngle[1] += 180 * direction;
			}
		}

	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ghost_vertex.size(), &ghost_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ghost_normal.size(), &ghost_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * ghost_vt.size(), &ghost_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색

		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);


		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glDrawArrays(GL_TRIANGLES, 0, ghost_vertex.size());
	}
};

struct make_mode {
	GLuint m_VBOVertex;
	GLuint m_VBONormal;
	GLuint m_VBOTexture;
	GLuint m_VBOColor;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 5.0f,5.0f,5.0f };
	vec3 color;
	vec3 move_pos;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	int m_modeN;

	make_mode(int num) {
		color.r = 1.0f;
		color.g = 1.0f;
		color.b = 1.0f;
		m_modeN = num;

		if (num == 0) {
			move_pos = { -12.0f,-2.3f,-15.0f };
			rotateAngle[1] = -90.0f;
		}
		else if (num == 1) {
			move_pos = { -12.0f,-2.3f,-15.0f };
			rotateAngle[1] = -90.0f;
		}
		SetVBO();
	}

	void SetVBO() {
		//vbo객체 생성
		glGenBuffers(1, &m_VBOVertex);
		glGenBuffers(1, &m_VBONormal);
		glGenBuffers(1, &m_VBOTexture);
	}

	void draw() {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mode_vertex.size(), &mode_vertex[0], GL_STATIC_DRAW);

		int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(positionAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBONormal);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mode_normal.size(), &mode_normal[0], GL_STATIC_DRAW);

		int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mode_vt.size(), &mode_vt[0], GL_STATIC_DRAW);

		int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
		//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
		glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
		glEnableVertexAttribArray(textureAttrib);


		model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);
		//model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);

		model_matrix = glm::translate(model_matrix, (move_pos));

		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
		model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

		model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
		model_matrix = parents_matrix * model_matrix;

		unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
		glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));


		unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform3f(objColorLocation, color.r, color.g, color.b);

		GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
		glUniform1i(textureNum, 1);

		if (m_modeN == 0) {
			glBindTexture(GL_TEXTURE_2D, texture[7]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		else if (m_modeN == 1) {
			glBindTexture(GL_TEXTURE_2D, texture[8]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	}
};

void Find_min_max(vec3& min, vec3& max, vec3& vertex)
{
	if (min.x > vertex.x)
		min.x = vertex.x;

	if (min.y > vertex.y)
		min.y = vertex.y;

	if (min.z > vertex.z)
		min.z = vertex.z;
	//---------------------//
	if (max.x < vertex.x)
		max.x = vertex.x;

	if (max.y < vertex.y)
		max.y = vertex.y;

	if (max.z < vertex.z)
		max.z = vertex.z;
}
void objectcollsion();
bool door_collision();
struct FACES
{
	vector <vec3> max_vertex;                                       //크기가 [1032]
	vector <vec3> min_vertex;                                       //크기가 [1032]
	//크기가 [1032]

	void initailize()
	{
		for (int i = 0; i < 1032; ++i)
		{
			vec3 temp_min; vec3 temp_max;
			temp_min = D_vertex[6 * i];
			temp_max = D_vertex[6 * i];
			for (int j = 0; j < 5; ++j)
			{
				Find_min_max(temp_min, temp_max, D_vertex[6 * i + j + 1]);

			}
			max_vertex.push_back(temp_max);
			min_vertex.push_back(temp_min);
		}
	}
};

vector <make_Player> Player;
vector <make_Player_check> Player_check;
vector <MAZE> maze;
vector <make_item> item;
vector <make_mode> mode;
vector <make_key> key;
vector <make_ghost> ghost;
vector <make_plane> plane;
vector <make_door> door;
vector <make_bottom> bottom;
vector <make_hint> hint;
CAMERA camera;
CAMERA minimap_camera;
vector <SPHERE> sphere;
FACES Face;


void main(int argc, char** argv) {//--- 윈도우 출력하고 콜백함수 설정
	//--------------------------------------------------------------------------------------------
	//if (!network.IsConnect()) return;
	//--------------------------------------------------------------------------------------------
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // 디스플레이 모드 설정

	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(g_width, g_height); // 윈도우의 크기 지정
	glutCreateWindow("OPENGL"); // 윈도우 생성
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		cerr << "Unable to initialize GLEW" << endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized" << endl;
	make_vertexShaders(); //--- 버텍스 세이더 만들기
	make_fragmentShaders(); //--- 프래그먼트 세이더 만들기
	shaderProgramID = make_shaderProgram();
	ReadObj("Maze_tri.obj", D_vertex, D_normal, D_vt);
	ReadObj("planeTP.obj", plane_vertex, plane_normal, plane_vt);
	ReadObj("Ghost.obj", ghost_vertex, ghost_normal, ghost_vt);
	ReadObj("cube.obj", item_vertex, item_normal, item_vt);
	ReadObj("key.obj", key_vertex, key_normal, key_vt);
	ReadObj("door.obj", door_vertex, door_normal, door_vt);
	ReadObj("sphere.obj", S_vertex, S_normal, S_vt);
	ReadObj("planeMode.obj", mode_vertex, mode_normal, mode_vt);

	ReadObj("Player1.obj", Player_vertex, Player_normal, Player_vt);
	ReadObj("Player_check.obj", Player_check_vertex, Player_check_normal, Player_check_vt);

	for (int i = 0; i < S_vertex.size(); ++i)
	{
		S_vertex[i].y += 0.5f;
	}
	maze.emplace_back(g_width / 2, g_height / 2);
	sphere.emplace_back(g_width / 2, g_height / 2);
	sphere[0].scale = 0.05f;
	plane.emplace_back(0);
	plane.emplace_back(1);
	bottom.emplace_back();
	door.emplace_back(1);
	door.emplace_back(2);
	door.emplace_back(3);
	door.emplace_back(4);
	key.emplace_back(0);
	key.emplace_back(1);
	key.emplace_back(2);
	mode.emplace_back(0);
	mode.emplace_back(1);

	Player.emplace_back(); //플레이어 생성
	Player_check.emplace_back(); //플레이어1P 생성

	for (int i = 0; i < 20; ++i) {
		ghost.emplace_back(i);
	}
	for (int i = 0; i < 20; ++i) {
		item.emplace_back(i);
	}
	hint.emplace_back();

	int width = 1024;
	int height = 1024;
	int numberOfChannel = 0;
	{
		stbi_set_flip_vertically_on_load(true);
		glGenTextures(10, texture);																					    //--- 텍스처 생성
		//벽돌 텍스쳐
		glBindTexture(GL_TEXTURE_2D, texture[0]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                        //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData1 = stbi_load("redBlock.jpg", &width, &height, &numberOfChannel, 4);                  //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData1);              //---텍스처 이미지 정의
		stbi_image_free(texutreData1);

		//랜덤 박스 텍스쳐
		glBindTexture(GL_TEXTURE_2D, texture[1]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                        //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData2 = stbi_load("RandomBox.jpg", &width, &height, &numberOfChannel, 4);               //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData2);             //---텍스처 이미지 정의
		stbi_image_free(texutreData2);

		glBindTexture(GL_TEXTURE_2D, texture[2]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData3 = stbi_load("wall.jpg", &width, &height, &numberOfChannel, 4);                  //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData3);            //---텍스처 이미지 정의
		stbi_image_free(texutreData3);

		glBindTexture(GL_TEXTURE_2D, texture[3]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData4 = stbi_load("sea.jpg", &width, &height, &numberOfChannel, 4);                  //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData4);            //---텍스처 이미지 정의
		stbi_image_free(texutreData4);

		glBindTexture(GL_TEXTURE_2D, texture[4]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData5 = stbi_load("door.jpg", &width, &height, &numberOfChannel, 4);                  //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData5);            //---텍스처 이미지 정의
		stbi_image_free(texutreData5);

		glBindTexture(GL_TEXTURE_2D, texture[5]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData6 = stbi_load("key.jpg", &width, &height, &numberOfChannel, 4);                  //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData6);            //---텍스처 이미지 정의
		stbi_image_free(texutreData6);

		glBindTexture(GL_TEXTURE_2D, texture[6]);                                                       //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);                                       //--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData7 = stbi_load("ghost.jpg", &width, &height, &numberOfChannel, 4);                  //맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData7);            //---텍스처 이미지 정의
		stbi_image_free(texutreData7);

		glBindTexture(GL_TEXTURE_2D, texture[7]);																	    //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);													//--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData8 = stbi_load("start.jpg", &width, &height, &numberOfChannel, 4);						//맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData8);				//---텍스처 이미지 정의
		stbi_image_free(texutreData8);

		glBindTexture(GL_TEXTURE_2D, texture[8]);																	    //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);													//--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData9 = stbi_load("end.jpg", &width, &height, &numberOfChannel, 4);						//맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData9);				//---텍스처 이미지 정의
		stbi_image_free(texutreData9);

		glBindTexture(GL_TEXTURE_2D, texture[9]);																	    //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);													//--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData10 = stbi_load("P1_check.jpg", &width, &height, &numberOfChannel, 4);						//맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData10);				//---텍스처 이미지 정의
		stbi_image_free(texutreData10);

		glBindTexture(GL_TEXTURE_2D, texture[10]);																	    //--- 텍스처 바인딩
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);													//--- 현재 바인딩된 텍스처의 파라미터 설정하기
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		unsigned char* texutreData11 = stbi_load("Player1.jpg", &width, &height, &numberOfChannel, 4);						//맨 마지막 인자가 너비가 4픽셀이 되게 하는 인자
		//jpg니까 GL_RGBA로 로드
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texutreData11);				//---텍스처 이미지 정의
		stbi_image_free(texutreData11);
	}

	glBindTexture(GL_TEXTURE_2D, 0); // 텍스처 바인딩 해제


	maze[0].makeBB(D_vertex);
	glutTimerFunc(17, Timer, CamMove);
	light_minimap.lightpos = { 0,28,0 };
	glutTimerFunc(17, Timer, 1);
	glutTimerFunc(17, Timer, 2);
	glutTimerFunc(34, Timer, 3);
	glutTimerFunc(34, Timer, 4);
	glutTimerFunc(17, Timer, 5);
	glutPassiveMotionFunc(Motion);
	glutKeyboardFunc(KeyBoardFunc);
	glutTimerFunc(17, Timer, CamMove);
	glutKeyboardUpFunc(KeyBoardUpFunc);
	glutTimerFunc(20, Timer, Live);
	glutDisplayFunc(drawScene); //--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutMainLoop(); // 이벤트 처리 시작
}

bool maze_collision()
{
	int i = 0;
	for (auto a : maze[0].bounding_box)
	{
		if ((a.Min_coord.x - 0.08 < camerapos.x && camerapos.x < a.Max_coord.x + 0.08) && (a.Min_coord.y - 0.1 < camerapos.y && camerapos.y < a.Max_coord.y + 0.1)
			&& (a.Min_coord.z - 0.08 < camerapos.z && camerapos.z < a.Max_coord.z + 0.08))
		{
			cout << "인덱스 번호:" << i << endl;
			cout << "블록 X좌표:" << a.Min_coord.x << "     " << a.Max_coord.x << endl;
			cout << "블록 Y좌표:" << a.Min_coord.y << "     " << a.Max_coord.y << endl;
			cout << "블록 Z좌표:" << a.Min_coord.z << "     " << a.Max_coord.z << endl;
			cout << "충돌함" << endl;
			return TRUE;
		}
		else
		{
			i++;
			//cout << "충돌안함" << endl;
			//return FALSE;
		}
	}
	if (i == maze[0].bounding_box.size())
		return FALSE;

}


void drawScene()
{
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);
	ShowCursor(FALSE);
	//glEnable(GL_CULL_FACE);
	light.lightpos = camerapos;

	//glutWarpPointer(g_width / 2, g_height / 2);
	glViewport(0, 0, g_width, g_height);
	if (Mode == 0) {
		camera.rotate_camera();
		camera.Draw(camerapos, shaderProgramID, perspective_projection);
		mode[0].draw();
		//camera.Draw(camerapos, shaderProgramID, perspective_projection);
		light.draw(camerapos, shaderProgramID, FALSE);

		glClear(GL_DEPTH_BUFFER_BIT);
		//glViewport(g_width * 4 / 5, g_height * 4 / 5, g_width / 5, g_height / 5);
	}		//시작화면
	else if (Mode == 1) {			//인게임 화면
		light.draw(camerapos, shaderProgramID, light_button);
		camera.rotate_camera();
		camera.InGameDraw(camerapos, shaderProgramID, perspective_projection);
		maze[0].Draw(D_vertex, D_normal, D_vt, shaderProgramID);
		plane[0].draw();
		bottom[0].draw();
		plane[0].draw();
		plane[1].draw();

		//camerapos, camera.cameraAt를 인자로 받아와 주인공의 위치와 바라보는 방향을 정해줌.
		Player[0].Draw(camerapos, camera.cameraAt, Player_vertex, Player_normal, Player_vt, shaderProgramID);
		Player_check[0].Draw(camerapos, camera.cameraAt, Player_check_vertex, Player_check_normal, Player_check_vt, shaderProgramID);

		for (auto& doors : door)
		{
			doors.draw();
		}
		maze[0].Draw(D_vertex, D_normal, D_vt, shaderProgramID);


		for (int i = 0; i < item.size(); ++i) {
			if (item[i].exist) {
				item[i].draw();
			}
		}

		for (int i = 0; i < key.size(); ++i) {
			if (key[i].exist) {
				key[i].draw();
			}
		}

		if (item_navi == 1) {
			hint[0].draw();
		}

		for (int i = 0; i < ghost.size(); ++i) {
			ghost[i].draw();
		}

		if (howManyKey == 3 && camerapos.x < -13.0f) {
			camerapos.x = -12.0f;
			camerapos.y = 0.2f;
			camerapos.z = -9.5f;

			camera.cameraAt = { 0,0,-1 };
			Mode = 3;
			glutTimerFunc(17, Timer, 5);
		}

		sphere[0].move_pos = { camerapos.x,camerapos.y + 4.0f,camerapos.z };
		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(g_width * 4 / 5, g_height * 4 / 5, g_width / 5, g_height / 5);
		//==================================================//미니맵 부분

		if (button_m) {
			light_minimap.draw(camerapos, shaderProgramID, FALSE);
			vec3 temp = { camerapos.x,camerapos.y + 3.0f,camerapos.z };
			minimap_camera.cameraEye = temp;
			minimap_camera.cameraUp = { camera.cameraAt.x,camera.cameraAt.y + 3.0f,camera.cameraAt.z };
			minimap_camera.Draw(temp, shaderProgramID, perspective_projection);

		}
		else
		{
			light_minimap.draw(camerapos, shaderProgramID, FALSE);
			vec3 temp1 = { 0,25,0 };
			minimap_camera.cameraEye = temp1;
			minimap_camera.cameraAt = { 0,-1,0 };
			minimap_camera.cameraUp = { 1,0,0 };
			minimap_camera.Draw(temp1, shaderProgramID, orthogonal_projection);
		}
		maze[0].Draw(D_vertex, D_normal, D_vt, shaderProgramID);
		if (item_navi == 1) {
			hint[0].draw();
		}
		if (item_where == 1) {
			for (int i = 0; i < item.size(); ++i) {
				if (item[i].exist) {
					item[i].scale[0] = 3.0f;
					item[i].scale[1] = 3.0f;
					item[i].scale[2] = 3.0f;
					item[i].draw();
				}
			}
		}
		if (item_Key_where == 1) {
			for (int i = 0; i < key.size(); ++i) {
				if (key[i].exist) {
					key[i].scale[0] = 0.3f;
					key[i].scale[1] = 0.3f;
					key[i].scale[2] = 0.3f;
					key[i].draw();
				}
			}
		}

		for (int i = 0; i < item.size(); ++i) {
			item[i].scale[0] = 1.0f;
			item[i].scale[1] = 1.0f;
			item[i].scale[2] = 1.0f;
		}
		for (int i = 0; i < key.size(); ++i) {
			key[i].scale[0] = 0.05f;
			key[i].scale[1] = 0.05f;
			key[i].scale[2] = 0.05f;
		}
		for (int i = 0; i < ghost.size(); ++i) {
			ghost[i].scale[0] = 0.1f;
			ghost[i].scale[1] = 0.1f;
			ghost[i].scale[2] = 0.1f;
		}
		sphere[0].Draw(S_vertex, S_normal, S_vt, shaderProgramID);
		plane[0].draw();
		maze[0].Draw(D_vertex, D_normal, D_vt, shaderProgramID);
		for (int i = 0; i < ghost.size(); ++i) {
			ghost[i].draw();
		}

		//======================================================//
	}
	else if (Mode == 3) {
		//camera.rotate_camera();
		camera.cameraAt = { 0.0,0.0,-1.0f };
		camera.Draw(camerapos, shaderProgramID, perspective_projection);
		mode[1].draw();

		glClear(GL_DEPTH_BUFFER_BIT);
		glViewport(g_width * 4 / 5, g_height * 4 / 5, g_width / 5, g_height / 5);
	}
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glutSwapBuffers(); //화면에 출력
}

GLvoid Timer(int value)
{
	switch (value) {
	case Live:
		objectcollsion();
		glutPostRedisplay();
		glutTimerFunc(17, Timer, 0);
		break;

	case CamMove:

		for (int i = 0; i < ghost.size() / 2; ++i)
		{
			ghost[i].Move(1);
		}

		for (int i = (ghost.size() / 2); i < ghost.size(); ++i)
		{
			ghost[i].Move(2);
		}

		switch (camera.direction)
		{
		case Up:
			camerapos.x += camera.speed * camera.cameraAt.x;
			if (creativemode)
			{
				camerapos.y += camera.speed * camera.cameraAt.y;
			}
			camerapos.z += camera.speed * camera.cameraAt.z;
			if ((maze_collision() || door_collision()) && !creativemode)
			{
				camerapos.x -= camera.speed * camera.cameraAt.x;
				camerapos.z -= camera.speed * camera.cameraAt.z;
			}
			break;
		case Down:
			camerapos.x -= camera.speed * camera.cameraAt.x;
			if (creativemode)
			{
				camerapos.y -= camera.speed * camera.cameraAt.y;
			}
			camerapos.z -= camera.speed * camera.cameraAt.z;
			if ((maze_collision() || door_collision()) && !creativemode)
			{
				camerapos.x += camera.speed * camera.cameraAt.x;
				camerapos.z += camera.speed * camera.cameraAt.z;
			}
			break;

		case Left:
			camerapos.x += camera.speed * camera.camerleft.x;
			if (creativemode)
			{
				camerapos.y += camera.speed * camera.camerleft.y;
			}
			camerapos.z += camera.speed * camera.camerleft.z;
			if ((maze_collision() || door_collision()) && !creativemode)
			{
				camerapos.x += camera.speed * camera.camerright.x;
				camerapos.z += camera.speed * camera.camerright.z;
			}
			break;

		case Right:
			camerapos.x += camera.speed * camera.camerright.x;
			if (creativemode)
			{
				camerapos.y += camera.speed * camera.camerright.y;
			}
			camerapos.z += camera.speed * camera.camerright.z;
			if ((maze_collision() || door_collision()) && !creativemode)
			{
				camerapos.x += camera.speed * camera.camerleft.x;
				camerapos.z += camera.speed * camera.camerleft.z;
			}

			break;
		default:
			break;
		}
		glutTimerFunc(17, Timer, 1);
		break;
	case 2:         // 아이템 회전
		for (int i = 0; i < 20; ++i) {
			item[i].rotateAngle[1] += 2.0f;
		}
		for (int i = 0; i < 3; ++i) {
			key[i].rotateAngle[1] += 2.0f;
		}
		glutTimerFunc(17, Timer, 2);
		break;
	case 3:         // 아이템 위 아래
		for (int i = 0; i < 20; ++i) {
			if (item[i].m_direction) {
				item[i].move_pos.y += 0.001f;
				item[i].m_direcN++;
				if (item[i].m_direcN == 20) {
					item[i].m_direction = false;
				}
			}
			else if (!item[i].m_direction) {
				item[i].move_pos.y -= 0.001f;
				item[i].m_direcN--;
				if (item[i].m_direcN == 0) {
					item[i].m_direction = true;
				}
			}
		}
		for (int i = 0; i < 3; ++i) {
			if (key[i].m_direction) {
				key[i].move_pos.y += 0.001f;
				key[i].m_direcN++;
				if (key[i].m_direcN == 20) {
					key[i].m_direction = false;
				}
			}
			else if (!item[i].m_direction) {
				key[i].move_pos.y -= 0.001f;
				key[i].m_direcN--;
				if (key[i].m_direcN == 0) {
					key[i].m_direction = true;
				}
			}
		}
		glutTimerFunc(34, Timer, 3);
		break;
	case 4:         // 문 회전
		if (camerapos.x >= -11.0f && camerapos.x <= -9.0f) {
			for (int i = 0; i < 2; ++i) {
				if (!door[i].m_IsOpen) {
					if (i % 2 == 0) {
						door[i].rotateAngle[1] += -1.0f;
						door[i].m_OpenN++;
						if (door[i].m_OpenN == 89) {
							door[i].m_IsOpen = 1;
						}
					}
					else if (i % 2 == 1) {
						door[i].rotateAngle[1] += 1.0f;
						door[i].m_OpenN++;
						if (door[i].m_OpenN == 89) {
							door[i].m_IsOpen = 1;
						}
					}

				}
			}
		}
		if (camerapos.x >= -10.0f && camerapos.x <= -9.0f && camerapos.z >= -7.0f && camerapos.z <= -6.0f) {
			if (howManyKey == 3) {
				for (int i = 2; i < 4; ++i) {
					if (!door[i].m_IsOpen) {
						if (i % 2 == 0) {
							door[i].rotateAngle[1] += 1.0f;
							door[i].m_OpenN++;
							if (door[i].m_OpenN == 89) {
								door[i].m_IsOpen = 1;
							}
						}
						else if (i % 2 == 1) {
							door[i].rotateAngle[1] += -1.0f;
							door[i].m_OpenN++;
							if (door[i].m_OpenN == 89) {
								door[i].m_IsOpen = 1;
							}
						}

					}
				}
			}
		}
		glutTimerFunc(34, Timer, 4);
		break;
	case 5:
		switch (Mode)
		{
		case 0:
			PlaySound(TEXT("startsound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			break;
		case 3:

			PlaySound(TEXT("stageclear.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			break;
		default:
			break;
		}


		break;

	}
}

GLvoid KeyBoardFunc(unsigned char key, int x, int y)

{

	switch (key)
	{
	case 'p':
		creativemode = !creativemode;
		light_button = !light_button;
		if (creativemode)
		{
			CMPOS = { camerapos.x,camerapos.y,camerapos.z };
		}
		else
			camerapos = CMPOS;
		break;
	case 'd':
		if (Mode == 1) {
			camera.direction = Right;
			//Player[0].direction = Right;
		}
		break;
	case 'a':
		if (Mode == 1) {
			camera.direction = Left;
			//Player[0].direction = Left;
		}
		break;
	case 'w':
		if (Mode == 1) {
			camera.direction = Up;
			Player[0].direction = Up;
		}
		break;
	case 's':
		if (Mode == 1) {
			camera.direction = Down;
			//Player[0].direction = Down;
		}
		break;
	case'm':
		button_m = !button_m;
		break;
	case 'y':
		camerapos.y += 0.1f;
		break;
	case'Y':
		camerapos.y -= 0.1f;
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	case 32:
		if (Mode == 0)
			PlaySound(NULL, NULL, 0);
		Mode = 1;
		break;
	case '+':
		크리에이티브모드 = true;
		item_navi = 1;
		item_where = 1;
		item_Key_where = 1;
		break;
	case '-':
		크리에이티브모드 = false;
		item_navi = 0;
		item_where = 0;
		item_Key_where = 1;
		break;
	}

	if (door_collision())
		cout << "카메라x좌표:" << camerapos.x << "y좌표:" << camerapos.y << "z좌표:" << camerapos.z << endl;
	else
	{

	}
	/*if (AABBcollision()) {
	   printf("충돌했습니다!!\n");
	}
	else
	   printf("충돌 안함\n");*/
	glutPostRedisplay();

}

GLvoid KeyBoardUpFunc(unsigned char key, int x, int y)
{
	// 나머지 키들의 로직도 유사하게 추가
	camera.direction = Stop;
	Player[0].direction = Stop;
}


GLvoid Motion(int x, int y)
{

	if (Mode > 0) {
		camera.Move(x, y);
		glutWarpPointer(g_width / 2, g_height / 2);
	}
	glutPostRedisplay();
}

void objectcollsion()
{
	for (int i = 0; i < item.size(); ++i) {
		if (pow((item[i].move_pos.x - camerapos.x), 2) + pow((item[i].move_pos.z - camerapos.z), 2) + pow((item[i].move_pos.y - camerapos.y), 2) < 0.04 && item[i].exist == true && 크리에이티브모드 == false) {   //아이템 먹기
			item[i].exist = false;
			PlaySound(TEXT("itemsound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);

			item[i].ability();
		}
		else if (크리에이티브모드 == true) {
			item[19].ability();
		}
	}

	for (int i = 0; i < key.size(); ++i) {
		if (pow((key[i].move_pos.x - camerapos.x), 2) + pow((key[i].move_pos.z - camerapos.z), 2) + pow((key[i].move_pos.y - camerapos.y), 2) < 0.04 && key[i].exist == true) {   //아이템 먹기
			key[i].exist = false;
			PlaySound(TEXT("keysound.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
			howManyKey++;
		}
	}
	if (!creativemode) {
		for (int i = 0; i < ghost.size(); ++i) {
			if (pow((ghost[i].move_pos.x - camerapos.x), 2) + pow((ghost[i].move_pos.z - camerapos.z), 2) + pow((ghost[i].move_pos.y - camerapos.y), 2) < 0.01) {   //아이템 먹기
				camerapos = { -12.0f,0.2f,-9.5f };

			}
		}
	}




}
bool door_collision()
{
	int num = 0;
	for (int i = 0; i < door.size(); ++i) {
		glm::vec4 doorMin4 = glm::vec4(vec3(-0.15, 0, -1), 1.0f); // 4차원 벡터로 변환
		glm::vec4 doorMax4 = glm::vec4(vec3(0.15, 1, 0), 1.0f);
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(door[i].rotateAngle[1]), AXIS_Y); // 회전 변환 행렬 적용
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), door[i].move_pos); // 회전 변환 행렬 적용
		doorMin4 = transMatrix * rotationMatrix * doorMin4;
		doorMax4 = transMatrix * rotationMatrix * doorMax4;

		glm::vec3 rotatedDoorMinPoint = glm::vec3(doorMin4); // 다시 3차원 벡터로 변환
		glm::vec3 rotatedDoorMaxPoint = glm::vec3(doorMax4);
		if (rotatedDoorMaxPoint.x < rotatedDoorMinPoint.x)
			swap(rotatedDoorMaxPoint.x, rotatedDoorMinPoint.x);

		if (rotatedDoorMaxPoint.y < rotatedDoorMinPoint.y)
			swap(rotatedDoorMaxPoint.y, rotatedDoorMinPoint.y);

		if (rotatedDoorMaxPoint.z < rotatedDoorMinPoint.z)
			swap(rotatedDoorMaxPoint.z, rotatedDoorMinPoint.z);


		// 카메라와 회전된 모양의 경계 상자 간 충돌 검사
		if (rotatedDoorMinPoint.x <= camerapos.x && rotatedDoorMaxPoint.x >= camerapos.x &&
			rotatedDoorMinPoint.y <= camerapos.y && rotatedDoorMaxPoint.y >= camerapos.y &&
			rotatedDoorMinPoint.z <= camerapos.z && rotatedDoorMaxPoint.z >= camerapos.z)
		{
			return TRUE;
		}
		else
			++num;
	}
	if (num == 4)
	{
		return FALSE;
	}
}
GLvoid Reshape(int w, int h) {//--- 콜백 함수: 다시 그리기 콜백 함수
	g_width = w;
	g_height = h;
	glViewport(0, 0, w, h);
}
void make_vertexShaders() {
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex_light.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result) {//result == 0이면 오류가 있다.
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShaders() {
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment_light.glsl");
	int fIdx = 0;
	while (fragmentSource[fIdx] != '\0') { // '\0' 문자까지 반복
		if (fragmentSource[fIdx] == '#') { // '#' 문자를 만나면 탈출
			fIdx++;
			break;
		}
		fIdx++;
	}
	const char* temp;
	temp = &fragmentSource[fIdx-1];
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &temp, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	cout << temp;
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
GLuint make_shaderProgram() {
	GLuint shaderID;
	shaderID = glCreateProgram(); //--- 세이더 프로그램 만들기 - 두 세이더 붙어야됨, vertex - fragment는 짝이 맞아야됨
	glAttachShader(shaderID, vertexShader); //--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderID, fragmentShader); //--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderID); //--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader); //--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		cerr << "ERROR: shader program 연결 실패\n" << errorLog << endl;
		return false;
	}
	glUseProgram(shaderID);
	return shaderID;
}