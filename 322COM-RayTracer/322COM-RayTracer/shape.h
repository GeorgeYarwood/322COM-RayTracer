#pragma once
#include <glm/glm.hpp>
using namespace glm;

class shape
{
private: 

public:
	shape();
	virtual vec3 getCenter();
	virtual vec3 getMyColor();

};

