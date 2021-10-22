#pragma once
#include "shape.h"
class AABB
{
    glm::vec3 pMin;
    glm::vec3 pMax;

    shape* currShape;

    AABB(glm::vec3 min, glm::vec3 max, shape &shape);

    virtual bool intersection(vec3& orig, vec3& dir, rayHit& hit);

};

