#pragma once
#include <glm/glm.hpp>
using namespace glm;

class shape
{
private: 
	vec3 centre;
	vec3 diffuseColour;
	vec3 diffuseIntesity;

public:
	shape();
	virtual bool intersection(vec3 rayDirection, vec3 rayOrigin, float& t, vec3& IntPt, vec3& normVec);
	virtual void ComputeColour(const float ambientIntensity, const vec3 IntPt, const vec3 lightPt, const vec3 RayDirection, float& ColValue);
	vec3 position;
	vec3 mcolour;

};

