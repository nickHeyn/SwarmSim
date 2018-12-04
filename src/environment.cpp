#include "environment.h"

Environment::Environment(int numAgents, GLuint shader) {
	srand(time(NULL));
	for (int i = 0; i < numAgents; i++) {
		agents.push_back(Agent(randomPosition(), randomVelocity(), shader));
	}
	shaderProgram = shader;
}

void Environment::updateAgents(float time) {
	for (int i = 0; i < agents.size(); i++) {
		agents[i].updateVectors(time, &agents);
	}
}

void Environment::Draw(Camera* cam, float asp, bool debug) {
	// draw agents
	for (int i = 0; i < agents.size(); i++) {
		agents[i].Draw(cam, asp);
	}

	if (debug) {
		// draw debug cube
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(CONTAIN_RADIUS*2, CONTAIN_RADIUS*2, CONTAIN_RADIUS*2));
		GLint uniModel = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		glm::mat4 view = glm::lookAt(
			cam->pos,  //Cam Position
			cam->getLookAt(),  //Look at point
			cam->up); //Up
		GLint uniView = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		glm::mat4 proj = glm::perspective(3.14f / 4, asp, 1.0f, 50.0f);
		//FOV, aspect ratio, near, far
		GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		GLint color = glGetUniformLocation(shaderProgram, "inColor");
		glm::vec3 colVec(.5, .5, .5);
		glUniform3fv(color, 1, glm::value_ptr(colVec));

		GLint uniTexID = glGetUniformLocation(shaderProgram, "texID");
		glUniform1i(uniTexID, -2);

		glDrawArrays(GL_TRIANGLES, 0, 36); //Number of vertices
	}
}

float* Environment::getModelData() {
	float* result = new float[NUM_MODEL_VERTS * 8];
	std::ifstream cubeFile;
	int currentIndex = 0;

	// First add the cube model
	cubeFile.open("models/cube.txt");
	int numLines = 0;
	cubeFile >> numLines;
	for (int i = 0; i < numLines; i++) {
		cubeFile >> result[currentIndex++];
	}

	std::ifstream sphereFile;
	// First add the sphere model
	sphereFile.open("models/sphere.txt");
	numLines = 0; 
	sphereFile >> numLines;
	for (int i = 0; i < numLines; i++) {
		sphereFile >> result[currentIndex++];
	}

	return result;
}

glm::vec3 Environment::randomPosition() {
	float x = ((float(rand()) / RAND_MAX) * 10) - 5;
	float y = ((float(rand()) / RAND_MAX) * 10) - 5;
	float z = ((float(rand()) / RAND_MAX) * 10) - 5;
	return glm::vec3(x, y, z);
}

glm::vec3 Environment::randomVelocity() {
	float x = ((float(rand()) / RAND_MAX) * 2) - 1;
	float y = ((float(rand()) / RAND_MAX) * 2) - 1;
	float z = ((float(rand()) / RAND_MAX) * 2) - 1;
	float velocity = (float((rand()) / RAND_MAX) * 6) - 3;
	glm::vec3 result(rand(), rand(), rand());
	if (velocity < MIN_SPEED && velocity > -MIN_SPEED) {
		if (velocity < 0)
			velocity = -MIN_SPEED;
		else
			velocity = MIN_SPEED;
	}
	return (glm::normalize(result)*velocity);
}