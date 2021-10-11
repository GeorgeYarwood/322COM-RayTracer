#pragma once
#include "shape.h"

class Sphere :
	public shape
{
private:
	float radius;
	vec3 centre;
public:
	Sphere(vec3 pos, float rad, vec3 col);
	~Sphere();
	float getRadius();
	bool intersection(vec3 rayDirection, vec3 rayOrigin, float& t, vec3& IntPt, vec3& normVec) override;


	void ComputeColour(const float ambientIntensity, const vec3 IntPt, const vec3 lightPt, const vec3 RayDirection, float& ColValue) override;
	


	
};