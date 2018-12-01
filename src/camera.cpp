#include "camera.h"

Camera::Camera() {
	pos = glm::vec3(10, 10, 10);
	up = glm::vec3(0, 1, 0);
}

glm::vec3 Camera::getLookAt() {
	return glm::vec3(0, 0, 0);
}