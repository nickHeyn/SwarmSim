#include "environment.h"

Environment::Environment(int numAgents, GLuint shader) {
	srand(time(NULL));
	shaderProgram = shader;
	for (int i = 0; i < numAgents; i++) {
		addAgent();
	}
}

void Environment::updateAgents(float time) {
	for (int i = 0; i < agents.size(); i++) {
		agents[i].updateVectors(time, &agents, predatorAgent);
	}
	if (predatorAgent != NULL) {
		if (predatorAgent->move(time)) {
			delete(predatorAgent);
			predatorAgent = NULL;
		}
	}
}

void Environment::Draw(Camera* cam, float asp, bool debug) {
	// draw agents
	for (int i = 0; i < agents.size(); i++) {
		agents[i].Draw(cam, asp);
	}
	if (predatorAgent != NULL) {
		// draw the predator agent if its released
		predatorAgent->Draw(cam, asp);
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

	// add the cube model
	cubeFile.open("models/cube.txt");
	int numLines = 0;
	cubeFile >> numLines;
	for (int i = 0; i < numLines; i++) {
		cubeFile >> result[currentIndex++];
	}

	// add the basic fish model
	readFishObjFile(currentIndex, result);

	return result;
}

glm::vec3 Environment::randomPosition() {
	float x = ((float(rand()) / RAND_MAX) * CONTAIN_RADIUS*2) - CONTAIN_RADIUS;
	float y = ((float(rand()) / RAND_MAX) * CONTAIN_RADIUS*2) - CONTAIN_RADIUS;
	float z = ((float(rand()) / RAND_MAX) * CONTAIN_RADIUS*2) - CONTAIN_RADIUS;
	return glm::vec3(x, y, z);
}

glm::vec3 Environment::randomVelocity() {
	float x = ((float(rand()) / RAND_MAX) * 2) - 1;
	float y = ((float(rand()) / RAND_MAX) * 2) - 1;
	float z = ((float(rand()) / RAND_MAX) * 2) - 1;
	float velocity = (float((rand()) / RAND_MAX) * MAX_SPEED*2) - MAX_SPEED;
	glm::vec3 result(rand(), rand(), rand());
	if (velocity < MIN_SPEED && velocity > -MIN_SPEED) {
		if (velocity < 0)
			velocity = -MIN_SPEED;
		else
			velocity = MIN_SPEED;
	}
	return (glm::normalize(result)*velocity);
}

void Environment::readFishObjFile(int currentIndex, float * result) {
	// key obj file
	std::vector< unsigned int > vertexI, normalI;
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec3 > normals;

	FILE* file = fopen("models/fish.obj", "r");
	while (true) {
		char header[100];
		int res = fscanf(file, "%s", header);
		if (res == EOF)
			break;
		if (strcmp(header, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(header, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}
		else if (strcmp(header, "f") == 0) {
			int vIndex[3], nIndex[3];
			fscanf(file, "%d//%d %d//%d %d//%d\n", &vIndex[0], &nIndex[0], &vIndex[1], &nIndex[1], &vIndex[2], &nIndex[2]);
			vertexI.push_back(vIndex[0]);
			vertexI.push_back(vIndex[1]);
			vertexI.push_back(vIndex[2]);

			normalI.push_back(nIndex[0]);
			normalI.push_back(nIndex[1]);
			normalI.push_back(nIndex[2]);
		}
	}
	for (int i = 0; i < vertexI.size(); i++) {
		result[currentIndex++] = (vertices[vertexI[i] - 1]).x;
		result[currentIndex++] = (vertices[vertexI[i] - 1]).y;
		result[currentIndex++] = (vertices[vertexI[i] - 1]).z;
		result[currentIndex++] = 0;
		result[currentIndex++] = 0;
		result[currentIndex++] = (normals[normalI[i] - 1]).x;
		result[currentIndex++] = (normals[normalI[i] - 1]).y;
		result[currentIndex++] = (normals[normalI[i] - 1]).z;
	}
}

void Environment::addAgent() {
	agents.push_back(Agent(randomPosition(), randomVelocity(), shaderProgram));
}

void Environment::releasePredatorAgent() {
	if (predatorAgent == NULL) {
		predatorAgent = new Predator(shaderProgram);
	}
}