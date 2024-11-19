#include "stdafx.h"
#include "maze.h"

extern unsigned int texture[4];

MAZE::MAZE(int x, int y)
{

	auto center = ConvertWinToGL(x, y);
	random_device rd;
	default_random_engine dre(rd());
	uniform_real_distribution<float> urd(0, 1);
	color.r = 0.2;
	color.g = 0.05;
	color.b = 0.0;
	InitVbo();

}

void MAZE::InitVbo() {
	glGenBuffers(1, &g_vbo); // VBO ����
	glGenBuffers(1, &g_vbo_normal); // VBO ����
	glGenBuffers(1, &m_VBOTexture); // VBO ����
}


void MAZE::Draw(vector<vec3>& D_vertex, vector<vec3>& D_normal, vector<vec2>& D_vt, GLuint& shaderProgramID)

{
	glBindBuffer(GL_ARRAY_BUFFER, g_vbo); // VBO�� ���ε�
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * D_vertex.size(), &D_vertex[0], GL_STATIC_DRAW);
	int vertexAttribLoc = glGetAttribLocation(shaderProgramID, "vPos");
	glVertexAttribPointer(vertexAttribLoc, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(vertexAttribLoc);

	glBindBuffer(GL_ARRAY_BUFFER, g_vbo_normal); // VBO�� ���ε�
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * D_normal.size(), &D_normal[0], GL_STATIC_DRAW);
	int noramldata = glGetAttribLocation(shaderProgramID, "vNormal");
	glVertexAttribPointer(noramldata, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(noramldata);


	glBindBuffer(GL_ARRAY_BUFFER, m_VBOTexture);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * D_vt.size(), &D_vt[0], GL_STATIC_DRAW);

	int textureAttrib = glGetAttribLocation(shaderProgramID, "vTexCoord");
	//� ��Ʈ��������, ���̴����� vec3�� 3(���� ����), ������ Ÿ��, ����ȭ, �ϳ��� ��� ũ��?, ���� ��ġ   
	glVertexAttribPointer(textureAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);             //--- ��ġ �Ӽ�
	glEnableVertexAttribArray(textureAttrib);


	model_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(gimbalAngle[2]), AXIS_Z);
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

	unsigned int objColorLocation = glGetUniformLocation(shaderProgramID, "objectColor"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform3f(objColorLocation, color.r, color.g, color.b);

	GLint textureNum = glGetUniformLocation(shaderProgramID, "TextureN"); //--- object Color�� ����: (1.0, 0.5, 0.3)�� ��
	glUniform1i(textureNum, 1);
	glBindTexture(GL_TEXTURE_2D, framework->texture[2]);
	glDrawArrays(GL_TRIANGLES, 0, D_vertex.size() - 12);

	glBindTexture(GL_TEXTURE_2D, framework->texture[0]);
	glUniform1i(textureNum, 1);
	glUniform3f(objColorLocation, 1, 0, 0);
	glDrawArrays(GL_TRIANGLES, D_vertex.size() - 6, 6);


}


void MAZE::makeBB(vector<vec3>& D_vertex)
{
	for (int i = 0; i < 2064; ++i)
	{
		Bounding_box temp;
		temp.Max_coord = D_vertex[3 * i];
		temp.Min_coord = D_vertex[3 * i];
		for (int j = 1; j < 3; ++j)
		{
			if (D_vertex[3 * i + j].x > temp.Max_coord.x)
			{
				temp.Max_coord.x = D_vertex[3 * i + j].x;
			}

			if (D_vertex[3 * i + j].y > temp.Max_coord.y)
			{
				temp.Max_coord.y = D_vertex[3 * i + j].y;
			}

			if (D_vertex[3 * i + j].z > temp.Max_coord.z)
			{
				temp.Max_coord.z = D_vertex[3 * i + j].z;
			}

			///////////////////////////////////////////////////////////////
			if (D_vertex[3 * i + j].x < temp.Min_coord.x)
			{
				temp.Min_coord.x = D_vertex[3 * i + j].x;
			}

			if (D_vertex[3 * i + j].y < temp.Min_coord.y)
			{
				temp.Min_coord.y = D_vertex[3 * i + j].y;
			}

			if (D_vertex[3 * i + j].z < temp.Min_coord.z)
			{
				temp.Min_coord.z = D_vertex[3 * i + j].z;
			}


		}
		bounding_box.push_back(temp);
	}
}