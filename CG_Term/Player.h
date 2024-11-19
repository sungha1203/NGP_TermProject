#pragma once
using namespace std;
using namespace glm;

class make_Player 
{
public:
	Direction direction = Stop;
	make_Player();
	void InitVbo();
	void Draw(glm::vec3, glm::vec3 cameraAt, vector<vec3>& D_vertex, vector<vec3>& D_normal, vector<vec2>& D_vt, GLuint& shaderProgramID);
private:
	GLuint p_vbo;
	GLuint p_vbo_normal;
	GLuint p_VBOTexture;

	float rotateAngle[3] = { 0 };
	float revolutionAngle[3] = { 0 };
	float scale[3] = { 0.007f,0.007f,0.007f };
	vec3 color;
	vec3 move_pos = { 0.0f,0.0f,0.0f };

	glm::mat4 model_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	glm::mat4 parents_matrix = glm::mat4(1.0f); //단위행렬로 초기화
	vector <Bounding_box> bounding_box;
};