#include "sphere.h"

Sphere::Sphere(vec3 pos, float rad, vec3 col) {
	currPos = pos;
	currRad = rad;
	currColour = col;
}

Sphere::~Sphere()
{
}

vec3 Sphere::getPos()
{
	return currPos;
}


vec3 Sphere::getColour()
{
	return currColour;
}

float Sphere::getRad()
{
	return currRad;
}

bool Sphere::intersection()
{
	return true;

}

void Sphere::ComputeColour()
{
	

	
}


