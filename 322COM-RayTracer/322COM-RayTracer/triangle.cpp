
#include "triangle.h"

triangle::triangle(vec3 v0, vec3 v1, vec3 v2, float u, float v, vec3 col, vec3 col1,vec3 col2, float diffIntensity, float specIntensity)
{
    currV0 = v0;
    currV1 = v1;
    currV2 = v2;
    currU = u;
    currV = v;
  
    currColour = col;
    currColour1 = col1;
    currColour2 = col2;

    currDiffuseIntensity = diffIntensity;
    currSpecIntensity = specIntensity;
}

bool triangle::intersection(vec3& orig, vec3& dir, rayHit& hit)
{

    // compute plane's normal
    vec3 v0v1 = currV1 - currV0;
    vec3 v0v2 = currV2 - currV0;
    // no need to normalize
    vec3 N = cross(v0v1, v0v2); // N 
    float denom = dot(N,N);


    // check if ray and plane are parallel ?
    float NdotRayDirection = dot(N, dir);
    if (fabs(NdotRayDirection) < 1e-8) // almost 0 
        return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = dot(N, currV0);

    // compute t (equation 3)
    hit.rayDist = (dot(N, orig) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (hit.rayDist < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    vec3 P = orig + hit.rayDist * dir;

    // Step 2: inside-outside test
    vec3 C; // vector perpendicular to triangle's plane 

    // edge 0
    vec3 edge0 = currV1 - currV0;
    vec3 vp0 = P - currV0;
    C = cross(edge0, vp0);
    if (dot(N,C) < 0) return false; // P is on the right side 

    // edge 1
    vec3 edge1 = currV2 - currV1;
    vec3 vp1 = P - currV1;
    C = cross(edge1, vp1);
    if ((currU = dot(N, C)) < 0)  return false; // P is on the right side 

    // edge 2
    vec3 edge2 = currV0 - currV2;
    vec3 vp2 = P - currV2;
    C = cross(edge2, vp2);
    if ((currV = dot(N, C)) < 0) return false; // P is on the right side; 

    currU /= denom;
    currV /= denom;

    return true; // this ray hits the triangle 

}

void triangle::ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir, vec3& ColValue)
{
    ColValue = currU * currColour + currV * currColour1 + (1 - currU - currV) * currColour2;
}

