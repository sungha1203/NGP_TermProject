#pragma once

enum type
{

	orthogonal_projection = 1,
	perspective_projection

};
extern float g_width;
extern float g_height;
class CAMERA {
public:
	Direction direction = Stop;
	glm::mat4 CR_matrix = glm::mat4(1.0f); //������ķ� �ʱ�ȭ
	float angle = 0.0f;
	const float speed = 0.015f;
	float mousePitch{ 0.0f };
	float mouseYaw{ 0.0f };
	glm::vec3 cameraEye;
	glm::vec3 cameraAt = glm::vec3(1, 0, 0); //--- ī�޶� �ٶ󺸴� ����
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //--- ī�޶� ���� ����
	glm::vec3 camerright;
	glm::vec3 camerleft;

	void Draw(glm::vec3 Camerapos, GLuint ID,type a);
	void InGameDraw(glm::vec3 Camerapos, GLuint ID, type a);
	void Move(float x, float y);
	void rotate_camera();
private:


};