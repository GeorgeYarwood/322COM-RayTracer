#pragma once
#include "shape.h"

class Sphere :
	public shape
{
	
public:
	Sphere(vec3 pos, float rad, vec3 col, float diffIntensity, float specIntensity);
	~Sphere();

	
	
	bool intersection(vec3& orig, vec3& dir, rayHit& hit) override;
	float currRad;
	//void ComputeColour(vec3 ambLightIntensity, float specIntensity, vec3 ambCol, vec3 diffColour, const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, vec3& ColValue) override;
	


	
};