#pragma once
#include <glm/glm.hpp>

using namespace glm;

class Sphere
{
private:
	float radius;
	vec3  center;
	vec3 mycolor;
public:
	Sphere(float, vec3, vec3);
	~Sphere();
	float getRadius(void);
	vec3 getCenter(void);
	vec3 getMyColor(void);
};