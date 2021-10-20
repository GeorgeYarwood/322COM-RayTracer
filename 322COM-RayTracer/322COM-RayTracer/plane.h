#pragma once
#include "shape.h"
class plane :
    public shape
{
    public:
        plane(vec3 col, vec3 point, vec3 normal);
        vec3 currNormal;
        vec3 currPoint;
        bool intersection(vec3& orig, vec3& dir, rayHit& hit) override;
        void ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir, vec3& ColValue) override;
};

