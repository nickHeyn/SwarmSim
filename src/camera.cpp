#include "camera.h"

Camera::Camera() {
	pos = glm::vec3(0, 0, 8);
	dir = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);
}

glm::vec3 Camera::getLookAt() {
	return pos + dir;
}

void Camera::updateCamera(float theta) {
	float x = dir.x;
	float z = dir.z;
	dir.x = cos(theta)*x - sin(theta)*z;
	dir.z = sin(theta)*x + cos(theta) * z;
	dir = glm::normalize(dir);
}