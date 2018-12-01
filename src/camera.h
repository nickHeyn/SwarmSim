#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class Camera
{
public:
	glm::vec3 pos, up;

	Camera();

	glm::vec3 getLookAt();

};
#endif // !CAMERA_H

