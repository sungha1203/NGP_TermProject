#pragma once

class Light {
public:
	glm::vec3 lightpos = { 0,5.0,0 };
	glm::vec3 lightRevol = { 0,0,0.0 };
	glm::vec3 color = { 1,1,1 };
	void draw(glm::vec3 camerapos, GLuint shaderProgramID, bool calculate);

private:


};


