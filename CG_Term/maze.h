#pragma once


using namespace std;
using namespace glm;
class MAZE {
public:
	GLuint g_vbo;
	GLuint g_vbo_normal;
	GLuint m_VBOTexture;
	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 1.0f,1.0f,1.0f };
	vec3 color;
	vec3 move_pos = { 0.0f,0.0f,0.0f };
	float gimbalAngle[3] = { 0 };
	bool Dodraw = TRUE;
	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	vector <Bounding_box> bounding_box;

	MAZE(int x, int y);
	
	void InitVbo();
	void Draw(vector<vec3>& D_vertex, vector<vec3>& D_normal, vector<vec2>& D_vt, GLuint& shaderProgramID);
	void makeBB(vector<vec3>& D_vertex);


};
