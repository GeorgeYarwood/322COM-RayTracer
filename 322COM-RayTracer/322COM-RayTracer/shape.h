#pragma once
#include <glm/glm.hpp>
using namespace glm;

class shape
{
private: 
	vec3 centre;
	vec3 diffuseColour;
	vec3 diffuseIntensity;
	float specIntensity;

public:
	shape();
	virtual bool intersection();
	virtual void ComputeColour();
	vec3 currPos;
	vec3 currColour;

};

