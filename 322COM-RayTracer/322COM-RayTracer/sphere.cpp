#include "sphere.h"

Sphere::Sphere(float r, vec3 cen, vec3 col) {
	radius = r;
	centre = cen;
	colour = col;
}

Sphere::~Sphere()
{
}

float Sphere::getRadius(void)
{
	return radius;
}

vec3 Sphere::getCenter()
{
	return centre;
}

vec3 Sphere::getMyColor()
{
	return colour;
}



