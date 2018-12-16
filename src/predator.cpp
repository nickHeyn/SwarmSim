#include "predator.h"

void Predator::Draw(Camera* c, float asp) {
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(PREDATOR_SCALE, PREDATOR_SCALE, PREDATOR_SCALE));
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::lookAt(
		c->pos,  //Cam Position
		c->getLookAt(),  //Look at point
		c->up); //Up
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 proj = glm::perspective(3.14f / 4, asp, 1.0f, 50.0f);
	//FOV, aspect ratio, near, far
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	GLint color = glGetUniformLocation(shaderProgram, "inColor");
	glm::vec3 colVec(1, 0, 1);
	glUniform3fv(color, 1, glm::value_ptr(colVec));

	GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
	glUniform1i(uniTexID, -1);

	glDrawArrays(GL_TRIANGLES, modelOffset, numVerts); //Number of vertices
}

Predator::Predator(GLuint shader) {
	shaderProgram = shader;
	position = glm::vec3(0, 0, CONTAIN_RADIUS + .5);
	direction = glm::vec3(0, 0, -1);
}

bool Predator::move(float dt) {
	position = position + direction * PREDATOR_SPEED * dt;
	return glm::length(position) > CONTAIN_RADIUS + .5;
}