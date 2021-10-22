#pragma once
#include "shape.h"
class AABB :
    public shape
{
    glm::vec3 pMin;
    glm::vec3 pMax;

    AABB(glm::vec3 min, glm::vec3 max);

    virtual bool intersection(vec3& orig, vec3& dir, rayHit& hit) override;

};

