#include "agent.h"

Agent::Agent(glm::vec3 pos, glm::vec3 dir, GLuint shader) {
	position = pos;
	direction = dir;
	shaderProgram = shader;
	maxSpeed = 3.0f;
	if (glm::length(direction) > maxSpeed) {
		direction = glm::normalize(direction) * maxSpeed;
	}
}

std::vector<Agent> Agent::getNearbyAgents(std::vector<Agent>* allAgents) {
	std::vector<Agent> result;
	for (int i = 0; i < allAgents->size(); i++) {
		Agent* currentAgent = &(*allAgents)[i];
		if (currentAgent == this)
			continue;
		if (glm::distance(currentAgent->position, position) <= NEARBY_RADIUS) {
			result.push_back(*currentAgent);
		}
	}
	return result;
}

void Agent::Draw(Camera* cam, float aspectRatio) {
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(.1, .1, .1));
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	glm::mat4 view = glm::lookAt(
		cam->pos,  //Cam Position
		cam->getLookAt(),  //Look at point
		cam->up); //Up
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glm::mat4 proj = glm::perspective(3.14f / 4, aspectRatio, 1.0f, 50.0f);
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

void Agent::updateVectors(float dt, std::vector<Agent>* allAgents) {
	// first calculate nearby agents, then calculate the new direction vector
	calculateDirection(getNearbyAgents(allAgents), dt);


	// finally, update the agent's position
	position = position + direction*dt;
}

void Agent::calculateDirection(std::vector<Agent> nearby, float dt) {
	direction = direction + getContainVector(dt);

	if (glm::length(direction) > maxSpeed) {
		direction = glm::normalize(direction) * maxSpeed;
	}
}

glm::vec3 Agent::getContainVector(float dt) {
	float dist = glm::length(position);
	if (dist > CONTAIN_RADIUS) {
		// need to redirect back to the origin
		float diff = dist - CONTAIN_RADIUS;
		float currentVelocity = glm::length(direction);
		glm::vec3 result = glm::normalize(-position);
		result = result * (dt * RETURN_ACCELERATION);
		return result;
	}
	// inside the containment area, return empty vector
	return glm::vec3(0, 0, 0);
}