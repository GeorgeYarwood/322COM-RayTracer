#pragma once
#include "shape.h"

class Sphere :
	public shape
{
private:
	float radius;
	vec3 centre;
	vec3 colour;
public:
	Sphere(float, vec3, vec3);
	~Sphere();
	float getRadius();
	vec3 getCenter() override;
	vec3 getMyColor() override;
	
};