#include "light.h"


light::light(glm::vec3 intensity, glm::vec3 pos)
{
	currIntensity = intensity;
	currPos = pos;
}