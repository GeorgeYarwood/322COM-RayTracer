#pragma once
#include "shape.h"

class Sphere :
	public shape
{
private:
	
	vec3 centre;
public:
	Sphere(vec3 pos, float rad, vec3 col, float diffIntensity, float specIntensity);
	~Sphere();

	vec3 getPos();
	vec3 getColour();
	float getRad();
	
	bool intersection(vec3& orig, vec3& dir, rayHit& hit) override;
	float currRad;

	void ComputeColour(vec3 ambLightIntensity, float specIntensity, vec3 ambCol, vec3 diffColour, const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, vec3& ColValue) override;
	


	
};