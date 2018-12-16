#pragma once
#ifndef AGENT_H
#define AGENT_H
#include "glad/glad.h"
#define GLM_FORCE_RADIANS //ensure we are using radians
#if defined(__APPLE__) || defined(__linux__)
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#else
#include <SDL.h>
#include <SDL_opengl.h>
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "camera.h"
#include "common.h"
#include "predator.h"
#include "math.h"
#include <vector>

class Agent
{
public:
	glm::vec3 position, direction;
	float speed;
	static const int modelOffset = 2916, numVerts = 348;
	float maxSpeed;
	GLuint shaderProgram;

	Agent(glm::vec3 pos, glm::vec3 dir, GLuint shader);

	// updates the position and direction vectors
	void updateVectors(float dt, std::vector<Agent>* allAgents, Predator* p);
	
	// draws the agent
	void Draw(Camera* c, float asp);

	// gets all the agents nearby and puts them in the returned list
	std::vector<Agent> getNearbyAgents(std::vector<Agent>* allAgents);

	// sums all the vectors to get a single direction vector
	void calculateDirection(std::vector<Agent> nearby, float dt, Predator* p);

	// keeps agents from colliding 
	glm::vec3 getSeparationVector(std::vector<Agent> nearby);

	// move to average position of neighboring agents
	glm::vec3 getCohesionVector(std::vector<Agent> nearby);

	// gets the average velocities of neighboring agents
	glm::vec3 getAlignmentVector(std::vector<Agent> nearby);

	// gets the vector that contains the agents in the "tank"
	glm::vec3 getContainVector(float dt);

	// random noise
	glm::vec3 getNoise();

	// makes the agents avoid the predator agent
	glm::vec3 getAvoid(Predator * p);

private:
	glm::mat4 getRotateMatrix();
};
#endif // !AGENT_H
