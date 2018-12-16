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
	glm::vec3 normalDirection = glm::normalize(direction);
	float theta_x = atan(normalDirection.y / normalDirection.z);
	float theta_y = atan(normalDirection.z / normalDirection.x);
	if (normalDirection.z < 0) {
		theta_x += 3.4159;
	}
	if (normalDirection.x < 0)
		theta_y += 3.14159;
	glm::mat4 model;
	model = glm::translate(model, position);
	model = model * getRotateMatrix();
	model = glm::scale(model, glm::vec3(AGENT_SCALE, AGENT_SCALE, AGENT_SCALE));
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
	glm::vec3 colVec(1,0,0);
	glUniform3fv(color, 1, glm::value_ptr(colVec));

	GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
	glUniform1i(uniTexID, -1);

	glDrawArrays(GL_TRIANGLES, modelOffset, numVerts); //Number of vertices
}

void Agent::updateVectors(float dt, std::vector<Agent>* allAgents, Predator * predatorAgent) {
	// calculate nearby agents, then calculate the new direction vector
	calculateDirection(getNearbyAgents(allAgents), dt, predatorAgent);


	// update the agent's position
	position = position + direction*dt;
}

void Agent::calculateDirection(std::vector<Agent> nearby, float dt, Predator * predatorAgent) {
	direction = direction + getContainVector(dt);
	direction = direction + getCohesionVector(nearby) * Common::cohesion_weight;
	direction = direction + getAlignmentVector(nearby) * Common::alignment_weight;
	direction = direction + getSeparationVector(nearby) * Common::separation_weight;
	direction = direction + getNoise() * Common::noise_weight;
	if (predatorAgent != NULL) {
		// only calculate if the predator agent is active
		direction = direction + getAvoid(predatorAgent);
	}

	// check if speed is within bounds
	if (glm::length(direction) > MAX_SPEED) {
		direction = glm::normalize(direction) * MAX_SPEED;
	}
	if (glm::length(direction) < MIN_SPEED) {
		direction = glm::normalize(direction) * MIN_SPEED;
	}
}

glm::vec3 Agent::getContainVector(float dt) {
	float dist = glm::length(position);
	if (dist + MOVE_FROM_WALL_DIST > CONTAIN_RADIUS) {
		glm::vec3 toOrigin = glm::normalize(-position);
		float diff = fabs(CONTAIN_RADIUS - dist);
		return toOrigin * (1.0f / diff);
	}
	return glm::vec3(0, 0, 0);
}

glm::vec3 Agent::getCohesionVector(std::vector<Agent> nearby) {
	if (nearby.size() == 0)
		return glm::vec3(0, 0, 0);
	glm::vec3 sum(0, 0, 0);
	for (int i = 0; i < nearby.size(); i++) {
		sum = sum + nearby[i].position;
	}
	sum = sum * (1.0f/nearby.size());
	return sum - position;
}

glm::vec3 Agent::getAlignmentVector(std::vector<Agent> nearby) {
	if (nearby.size() == 0)
		return glm::vec3(0, 0, 0);
	glm::vec3 sum(0, 0, 0);
	for (int i = 0; i < nearby.size(); i++) {
		sum = sum + nearby[i].direction;
	}
	sum = sum * (1.0f / nearby.size());
	return sum;
}

glm::vec3 Agent::getSeparationVector(std::vector<Agent> nearby) {
	glm::vec3 sum(0, 0, 0);
	for (int i = 0; i < nearby.size(); i++) {
		float dist = glm::distance(nearby[i].position, position);
		if (dist < CROWD_RADIUS) {
			glm::vec3 separation = glm::normalize(position - nearby[i].position);
			separation = separation * (1.0f / dist);
			sum = sum + separation;
		}
	}
	return sum;
}

glm::vec3 Agent::getNoise() {
	float x = ((float(rand()) / RAND_MAX) * 2) - 1;
	float y = ((float(rand()) / RAND_MAX) * 2) - 1;
	float z = ((float(rand()) / RAND_MAX) * 2) - 1;
	glm::vec3 result(rand(), rand(), rand());
	return (glm::normalize(result));
}

glm::mat4 Agent::getRotateMatrix() {
	/*
	Note: The following code was used from http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#foreword-rotation-vs-orientation
	*/
	glm::vec3 start(0, 0, -1);
	glm::vec3 dest = glm::normalize(direction);
	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;
	rotationAxis = glm::cross(start, dest);
	float s = std::sqrt((1 + cosTheta) * 2);
	float inverse = 1 / s;
	glm::quat rotationQuat(s*.5f, rotationAxis.x*inverse, rotationAxis.y*inverse, rotationAxis.z * inverse);
	return glm::toMat4(rotationQuat);
}

glm::vec3 Agent::getAvoid(Predator * predator) {
	float dist = glm::distance(predator->position, position);
	if (dist < PREDATOR_AWARENESS_RADIUS) {
		glm::vec3 avoidVector = glm::normalize(position - predator->position);
		return avoidVector * Common::avoid_weight;
	}
	else {
		return glm::vec3(0, 0, 0);
	}
}