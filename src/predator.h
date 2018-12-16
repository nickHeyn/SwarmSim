#pragma once
#ifndef PREDATOR_H
#define PREDATOR_H
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
#include "common.h"
#include "math.h"
#include <vector>

class Predator
{
public:
	glm::vec3 position, direction;
	static const int modelOffset = 2916, numVerts = 348;
	GLuint shaderProgram;

	Predator(GLuint shader);

	// draws the predator agent
	void Draw(Camera* c, float asp);

	// moves the predator agent through the screen. Returns true when it reaches the other side of the tank
	bool move(float dt);
};
#endif // !PREDATOR_H
