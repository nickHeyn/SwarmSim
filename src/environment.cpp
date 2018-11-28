#include "environment.h"

Environment::Environment(int numAgents, GLuint shader) {
	for (int i = 0; i < numAgents; i++) {
		agents.push_back(Agent(glm::vec3(0,0,0), glm::vec3(0,1,0), shader));
	}
}

void Environment::Draw(Camera* cam, float time, float asp) {
	for (int i = 0; i < agents.size(); i++) {
		agents[i].Draw(cam, time, asp);
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