#pragma once
#include "shape.h"
using namespace glm;
class triangle :
    public shape
{

public:
    triangle(vec3 pos, vec3 v0, vec3 v1, vec3 v2, vec3 col, vec3 col1, vec3 col2, vec3 norm0, vec3 norm1, vec3 norm2, float diffIntensity, float specIntensity);
    vec3 currColour1;
    vec3 currColour2;
    vec3 currV0;
    vec3 currV1;
    vec3 currV2;
    vec3 currNorm0;
    vec3 currNorm1;
    vec3 currNorm2;
    float currT;
    float currU;
    float currV;
    bool intersection(vec3& orig, vec3& dir, rayHit& hit) override;
    vec3 ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir) override;
    
};

