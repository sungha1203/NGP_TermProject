#include "stdafx.h"
#include "light.h"


void Light::draw(glm::vec3 camerapos, GLuint shaderProgramID,bool calculate)
{
	glm::mat4 lightMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(lightRevol.z), AXIS_Z);
	lightMatrix = glm::rotate(lightMatrix, glm::radians(lightRevol.y), AXIS_Y);
	lightMatrix = glm::rotate(lightMatrix, glm::radians(lightRevol.x), AXIS_X);
	lightMatrix = glm::translate(lightMatrix, (lightpos));
	unsigned int lightLocation = glGetUniformLocation(shaderProgramID, "lightMatix");		//lightpos임
	glUniformMatrix4fv(lightLocation, 1, GL_FALSE, glm::value_ptr(lightMatrix));

	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, color.r, color.g, color.b);

	unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos"); //--- viewPos 값 전달: 카메라 위치
	glUniform3f(viewPosLocation, camerapos.x, camerapos.y, camerapos.z);

	unsigned int distancecalculate = glGetUniformLocation(shaderProgramID, "cal_dis"); //--- viewPos 값 전달: 카메라 위치
	glUniform1i(glGetUniformLocation(shaderProgramID, "cal_dis"), calculate);




	
}