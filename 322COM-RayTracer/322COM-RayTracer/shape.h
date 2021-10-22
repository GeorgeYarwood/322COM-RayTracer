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
	virtual vec3 ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir);
	vec3 currPos;
	vec3 currColour;
	
	float currDiffuseIntensity;
	float currSpecIntensity;
	bool inShadow;

};

