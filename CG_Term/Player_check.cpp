#include "stdafx.h"
#include "Player_check.h"

extern unsigned int texture[10];

make_Player_check::make_Player_check()
{
	color.r = 1.0f;
	color.g = 1.0f;
	color.b = 1.0f;
	move_pos = { -12.0f,0.2f,-9.5f }; //�÷��̾� �ʱ���ġ
	InitVbo();
}

void make_Player_check::InitVbo()
{
	glGenBuffers(1, &p_vbo); // VBO ����
	glGenBuffers(1, &p_vbo_normal); // VBO ����
	glGenBuffers(1, &p_VBOTexture); // VBO ����
}

void make_Player_check::Draw(glm::vec3 pos, glm::vec3 cameraAt, vector<vec3>& D_vertex, vector<vec3>& D_normal, vector<vec2>& D_vt, GLuint& shaderProgramID)
{
	glBindBuffer(GL_ARRAY_BUFFER, p_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * D_vertex.size(), &D_vertex[0], GL_STATIC_DRAW);

	int positionAttrib = glGetAttribLocation(shaderProgramID, "vPos");
	//� ��Ʈ��������, ���̴����� vec3�� 3(���� ����), ������ Ÿ��, ����ȭ, �ϳ��� ��� ũ��?, ���� ��ġ   
	glVertexAttribPointer(positionAttrib, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(positionAttrib);

	glBindBuffer(GL_ARRAY_BUFFER, p_vbo_normal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * D_normal.size(), &D_normal[0], GL_STATIC_DRAW);

	int normalAttrib = glGetAttribLocation(shaderProgramID, "vNormal");
	//� ��Ʈ��������, ���̴����� vec3�� 3(���� ����), ������ Ÿ��, ����ȭ, �ϳ��� ��� ũ��?, ���� ��ġ   
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);             //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, p_VBOTexture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * D_vt.size(), &D_vt[0], GL_STATIC_DRAW);

	int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
	//� ��Ʈ��������, ���̴����� vec3�� 3(���� ����), ������ Ÿ��, ����ȭ, �ϳ��� ��� ũ��?, ���� ��ġ   
	glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(textureAttrib);


	// ī�޶� ���⿡�� Y�� ȸ�� ���� ���
	glm::vec3 cameraDirection = glm::normalize(cameraAt); // ī�޶��� �ü� ���� ����
	float cameraYaw = glm::degrees(atan2(cameraDirection.x, cameraDirection.z)); // Y�� ȸ�� ����


	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(revolutionAngle[0]), AXIS_X);
	pos.y += 0.02f;//���ΰ� ��ġ ��¦ ������
	model_matrix = glm::translate(model_matrix, (pos));

	model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[2]), AXIS_Z);
	model_matrix = glm::rotate(model_matrix, glm::radians(rotateAngle[0]), AXIS_X);
	model_matrix = glm::rotate(model_matrix, glm::radians(cameraYaw), AXIS_Y);

	model_matrix = glm::scale(model_matrix, glm::vec3(scale[0], scale[1], scale[2]));
	model_matrix = parents_matrix * model_matrix;

	unsigned int transLocation = glGetUniformLocation(shaderProgramID, "modelTransform");
	glUniformMatrix4fv(transLocation, 1, GL_FALSE, glm::value_ptr(model_matrix));

	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��

	glUniform3f(objColorLocation, color.r, color.g, color.b);

	GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform1i(textureNum, 1);


	glBindTexture(GL_TEXTURE_2D, texture[9]);
	glDrawArrays(GL_TRIANGLES, 0, D_vertex.size());
}
