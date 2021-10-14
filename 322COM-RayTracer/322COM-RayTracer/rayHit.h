#pragma once
#include <glm/glm.hpp>
struct rayHit {
	glm::vec3 intersectPoint;
	float rayDist;
	float ambientCol;
	float diffuseCol;
};