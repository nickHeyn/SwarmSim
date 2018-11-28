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
#include "camera.h"
#include <vector>

class Agent
{
public:
	glm::vec3 position, direction, color;
	float speed;
	static const int modelOffset = 36, numVerts = 2880;
	GLuint shaderProgram;

	Agent(glm::vec3 pos, glm::vec3 dir, GLuint shader);

	void updateVectors(float dt);
	
	void Draw(Camera* c, float time, float asp);
};
#endif // !AGENT_H
