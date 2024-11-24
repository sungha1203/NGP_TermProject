#pragma once
#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <utility>
#include "include/glew.h" // 필요한 헤더파일 include
#include "include/freeglut.h"
#include "include/freeglut_ext.h"
#include "include/glm/glm.hpp"
#include "include/glm/gtc/type_ptr.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/ext.hpp"
#include <string>
#include <functional>
#include <thread>
#include <mutex>
#include<DirectXCollision.h>
#pragma comment(lib, "freeglut")
#pragma comment(lib, "glew32")
#pragma comment(lib, "glew32s")

//extern float g_width;
//extern float g_height;
using namespace std;
using namespace glm;
using namespace DirectX;
enum timersort
{
	Live,
	CamMove
};
enum Direction
{
	Up,
	Down,
	Left,
	Right,
	Stop
};

char* filetobuf(const char* file);
void ReadObj(const std::string objfilename, std::vector<glm::vec3>& vertex, std::vector<glm::vec3>& normalVertex, std::vector<glm::vec2>& vtVertex);

enum  AXIS
{
	X,
	Y,
	Z
};
const glm::vec3 AXIS_X = glm::vec3(1, 0, 0);
const glm::vec3 AXIS_Y = glm::vec3(0, 1, 0);
const glm::vec3 AXIS_Z = glm::vec3(0, 0, 1);

std::pair<float, float> ConvertWinToGL(int x, int y);

struct Bounding_box
{
	glm::vec3 Max_coord;
	glm::vec3 Min_coord;
};