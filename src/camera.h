#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class Camera
{
public:
	// the position and up vector of the camera
	glm::vec3 pos, up;

	Camera();

	// returns a vector that gives the lookAt point
	glm::vec3 getLookAt();

};
#endif // !CAMERA_H

