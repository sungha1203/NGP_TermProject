#pragma once

using namespace std;
using namespace glm;


extern float g_width;
extern float g_height;
class SPHERE {
public:
	GLuint vbo;
	GLuint normal;
	GLuint texture;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale = 1.0f;
	glm::vec3 color;
	glm::vec3 move_pos = { 0.0f,0.0f,0.0f };
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화

	SPHERE(int x, int y);
	void InitVbo(); 
	void Draw(vector <glm::vec3>& S_vertex, vector <glm::vec3>& S_normal, vector <glm::vec2>& S_Uv, GLuint& shaderProgramID);

};