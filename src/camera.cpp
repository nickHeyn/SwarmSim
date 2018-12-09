#include "camera.h"

Camera::Camera() {
	pos = glm::vec3(10, 10, 10);
	up = glm::vec3(0, 1, 0);
	cameraSpeed = 2.5f;
}

glm::vec3 Camera::getLookAt() {
	return glm::vec3(0, 0, 0);
}

void Camera::rotateCamera(float dt, int dir) {
	float theta = dt * dir * cameraSpeed;
	float s = sin(theta);
	float c = cos(theta);
	float x_pos = pos.x * c - pos.z * s;
	float z_pos = pos.x * s + pos.z * c;
	pos.x = x_pos;
	pos.z = z_pos;
}