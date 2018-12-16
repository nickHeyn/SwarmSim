#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <time.h>
#include "predator.h"
#include "camera.h"
#include "common.h"
#include "agent.h"

class Environment
{
public:
	std::vector<Agent> agents;
	const static int NUM_MODEL_VERTS = 384;
	GLuint shaderProgram;
	Predator* predatorAgent;

	Environment(int numAgents, GLuint shaderProgram);

	void Draw(Camera* cam, float aspect, bool debug);

	float* getModelData();

	void updateAgents(float time);

	void addAgent();

	void releasePredatorAgent();

private:
	glm::vec3 randomPosition();

	glm::vec3 randomVelocity();

	void readFishObjFile(int index, float * resultArray);

};
#endif // !ENVIRONMENT_H

