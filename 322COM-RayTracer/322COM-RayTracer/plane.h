#pragma once
#include "shape.h"
class plane :
    public shape
{
private:
    vec3 point;
    vec3 normal;
public:
    plane(vec3 point, vec3 normal, vec3 col);


};

