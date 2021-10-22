#pragma once
#include "glm/glm.hpp"
class light
{
public:
	glm::vec3 currIntensity;
	glm::vec3 currPos;
	light(glm::vec3 intensity, glm::vec3 pos);
};

