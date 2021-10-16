#pragma once
#include <glm/glm.hpp>
#include "rayHit.h"
#include <fstream>     

using namespace glm;

class shape
{
private: 
	

public:
	shape();
	virtual bool intersection(vec3& orig, vec3& dir, rayHit& hit);
	virtual void ComputeColour(vec3 ambLightIntensity, float specIntensity, vec3 ambCol, vec3 diffColour, const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, vec3& ColValue);
	vec3 currPos;
	vec3 currColour;
	
	float currDiffuseIntensity;
	float currSpecIntensity;
};

