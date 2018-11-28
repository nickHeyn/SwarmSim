#pragma once
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "agent.h"
#include "camera.h"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
class Environment
{
public:
	std::vector<Agent> agents;
	const static int NUM_MODEL_VERTS = 2916;

	Environment(int numAgents, GLuint shaderProgram);

	void Draw(Camera* cam, float time, float aspect);

	float* getModelData();


};
#endif // !ENVIRONMENT_H

