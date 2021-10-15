#pragma once
#include <glm/glm.hpp>
struct rayHit {
	glm::vec3 intersectPoint;
	float rayDist;
	glm::vec3 ambientCol;
	glm::vec3 diffuseCol;
};