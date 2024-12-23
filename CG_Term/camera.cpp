#pragma once
#include "stdafx.h"
#include "camera.h"


void CAMERA::Draw(glm::vec3 Camerapos, GLuint ID, type a)
{

	cameraEye = { Camerapos }; //--- 카메라 위치

	camerright = glm::normalize(glm::cross(cameraUp, -cameraAt));
	camerleft = glm::normalize(glm::cross(cameraUp, +cameraAt));
	auto view = glm::lookAt(cameraEye, cameraEye + cameraAt, cameraUp);

	unsigned int viewLocation = glGetUniformLocation(ID, "viewTransform"); //--- 뷰잉 변환 설정
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	glm::mat4 projection = glm::mat4(1.0f);
	if (a == orthogonal_projection)
	{
		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -26.0f, 26.0f);
	}
	else
		projection = glm::perspective(glm::radians(45.0f), g_width / g_height, 0.1f, 100.0f); //--- 투영 공간 설정: fovy, aspect, near, far
	projection = projection * CR_matrix; //--- 공간을 z축 이동
	unsigned int projectionLocation = glGetUniformLocation(ID, "projectionTransform"); //--- 투영 변환 값 설정
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

void CAMERA::InGameDraw(glm::vec3 Camerapos, GLuint ID, type a)
{
	// 주인공을 기준으로 카메라 위치를 계산
	float cameraDistance = 0.5f;   // 주인공과 카메라 사이 거리
	//float heightOffset = 2.0f;     // 카메라 높이 오프셋

	// 3인칭 시점에서 카메라 위치와 방향 설정
	cameraEye = Camerapos - glm::normalize(cameraAt) * cameraDistance; // 주인공 뒤쪽에 위치
	//cameraEye.y += heightOffset;   // 카메라를 주인공보다 조금 위로 배치

	// 카메라가 주인공을 바라보도록 설정
	camerright = glm::normalize(glm::cross(cameraUp, -cameraAt));
	camerleft = glm::normalize(glm::cross(cameraUp, +cameraAt));
	auto view = glm::lookAt(cameraEye, Camerapos, cameraUp);

	// 뷰 변환 행렬을 셰이더로 전달
	unsigned int viewLocation = glGetUniformLocation(ID, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	// 투영 행렬 설정
	glm::mat4 projection = glm::mat4(1.0f);
	if (a == orthogonal_projection) {
		// 직교 투영 설정
		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -26.0f, 26.0f);
	}
	else {
		// 원근 투영 설정
		projection = glm::perspective(glm::radians(45.0f), g_width / g_height, 0.1f, 100.0f);
	}

	// CR_matrix와 함께 투영 행렬 조정
	projection = projection * CR_matrix;

	// 투영 변환 행렬을 셰이더로 전달
	unsigned int projectionLocation = glGetUniformLocation(ID, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projection[0][0]);
}

void CAMERA::Move(float x, float y)
{
	glm::vec2 prevPos = glm::vec2(500, 500);

	glm::vec2 pos = glm::vec2((float)x, (float)y);
	auto deltaPos = pos - prevPos;

	const float cameraRotspeed = 1.0f;
	mouseYaw -= deltaPos.x * cameraRotspeed;
	mousePitch -= deltaPos.y * cameraRotspeed;

	if (mouseYaw < 0.0f) mouseYaw += 360.0f;
	if (mouseYaw > 360.0f) mouseYaw -= 360.0f;

	if (mousePitch > 89.0f) mousePitch = 89.0f;
	if (mousePitch < -89.0f) mousePitch = -89.0f;

	prevPos = pos;
}
void CAMERA::rotate_camera()
{
	cameraAt =
		glm::rotate(glm::mat4(1.0f), glm::radians(mouseYaw), AXIS_Y) *
		glm::rotate(glm::mat4(1.0f), glm::radians(mousePitch), AXIS_X) *
		glm::vec4(0, 0, -1, 0);
}