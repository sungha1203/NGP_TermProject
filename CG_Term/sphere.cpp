#pragma once
#include "stdafx.h"
#include "sphere.h"

using namespace std;
using namespace glm;


SPHERE::SPHERE(int x, int y)
{
	auto center = ConvertWinToGL(x, y);
	/*random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urd(0, 1);*/
	color.r = 0;
	color.g = 1;
	color.b = 1;

	SPHERE::InitVbo();
}
void SPHERE::InitVbo() {
	glGenBuffers(1, &vbo); // VBO 생성
	glGenBuffers(1, &normal); // VBO 생성
	glGenBuffers(1, &texture); // VBO 생0성
}
void SPHERE::Draw(vector <vec3>& S_vertex, vector <vec3>& S_normal, vector <vec2>& S_Uv, GLuint& shaderProgramID) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // VBO를 바인딩
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * S_vertex.size(), &S_vertex[0], GL_STATIC_DRAW);
	int vertexAttribLoc = glGetAttribLocation(shaderProgramID, "vPos");
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(vertexAttribLoc);

	glBindBuffer(GL_ARRAY_BUFFER, normal); // VBO를 바인딩
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * S_normal.size(), &S_normal[0], GL_STATIC_DRAW);
	int noramldata = glGetAttribLocation(shaderProgramID, "vNormal");
	glVertexAttribPointer(noramldata, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(noramldata);


	glBindBuffer(GL_ARRAY_BUFFER, texture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * S_Uv.size(), &S_Uv[0], GL_STATIC_DRAW);

	int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
	//어떤 어트리브인지, 세이더에서 vec3면 3(변수 갯수), 데이터 타입, 정규화, 하나의 덩어리 크기?, 시작 위치   
	glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- 위치 속성
	glEnableVertexAttribArray(textureAttrib);

	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[2]), AXIS_Z);
	model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[0]), AXIS_X);
	model_matrix = glm::rotate(model_matrix, glm::radians(revolutionAngle[1]), AXIS_Y);


	model_matrix = glm::translate(model_matrix, (move_pos));

	model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
	model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
	model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[1]), AXIS_Y);

	model_matrix = glm::scale(model_matrix, glm::vec3(scale, scale, scale));
	model_matrix = parents_matrix * model_matrix;

	unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));
	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color값 전달: (1.0, 0.5, 0.3)의 색
	glUniform3f(objColorLocation, color.r, color.g, color.b);

	glDrawArrays(GL_TRIANGLES, 0, S_vertex.size());


}
