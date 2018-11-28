#include "agent.h"

Agent::Agent(glm::vec3 pos, glm::vec3 dir, GLuint shader) {
	position = pos;
	direction = dir;
	shaderProgram = shader;
}

void Agent::Draw(Camera* cam, float time, float aspectRatio) {
	glm::mat4 model;
	model = glm::translate(model, position);
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::lookAt(
		cam->pos,  //Cam Position
		cam->getLookAt(),  //Look at point
		cam->up); //Up
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 proj = glm::perspective(3.14f / 4, aspectRatio, 0.01f, 20.0f);
	//FOV, aspect ratio, near, far
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	GLint color = glGetUniformLocation(shaderProgram, "inColor");
	glm::vec3 colVec(1,0,1);
	glUniform3fv(color, 1, glm::value_ptr(colVec));

	GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
	glUniform1i(uniTexID, -1);

	glDrawArrays(GL_TRIANGLES, modelOffset, numVerts); //Number of vertices
}

void Agent::updateVectors(float dt) {
}