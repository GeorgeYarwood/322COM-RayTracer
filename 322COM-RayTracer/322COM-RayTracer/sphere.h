#pragma once
#include "shape.h"

class Sphere :
	public shape
{
private:
	
	vec3 centre;
public:
	Sphere(vec3 pos, float rad, vec3 col);
	~Sphere();

	vec3 getPos();
	vec3 getColour();
	float getRad();
	
	bool intersection() override;
	float currRad;

	void ComputeColour() override;
	


	
};