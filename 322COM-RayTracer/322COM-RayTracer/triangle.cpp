
#include "triangle.h"

triangle::triangle(vec3 pos, vec3 v0, vec3 v1, vec3 v2, float u, float v, vec3 col, vec3 col1,vec3 col2, vec3 norm0, vec3 norm1, vec3 norm2,float diffIntensity, float specIntensity)
{
    currPos = pos;

    currV0 = v0+ currPos;
    currV1 = v1+ currPos;
    currV2 = v2+ currPos;
    currU = u;
    currV = v;

    currNorm0 = norm0;
    currNorm1 = norm1;
    currNorm2 = norm2;
  
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
    if (abs(NdotRayDirection) < 0.00001) // almost 0 
        return false; // they are parallel so they don't intersect ! 

    // compute d parameter using equation 2
    float d = dot(N, currV0);

    // compute t (equation 3)
    hit.rayDist = (dot(N, orig) + d) / NdotRayDirection;
    // check if the triangle is in behind the ray
    if (hit.rayDist < 0) return false; // the triangle is behind 

    // compute the intersection point using equation 1
    hit.intersectPoint = orig + hit.rayDist * dir;

    // Step 2: inside-outside test
    vec3 C; // vector perpendicular to triangle's plane 

    // edge 0
    vec3 edge0 = currV1 - currV0;
    vec3 vp0 = hit.intersectPoint - currV0;
    C = cross(edge0, vp0);
    if (dot(N,C) < 0) return false; // P is on the right side 

    // edge 1
    vec3 edge1 = currV2 - currV1;
    vec3 vp1 = hit.intersectPoint - currV1;
    C = cross(edge1, vp1);
    if ((currU = dot(N, C)) < 0)  return false; // P is on the right side 

    // edge 2
    vec3 edge2 = currV0 - currV2;
    vec3 vp2 = hit.intersectPoint - currV2;
    C = cross(edge2, vp2);
    if ((currV = dot(N, C)) < 0) return false; // P is on the right side; 

    currU /= denom;
    currV /= denom;

    return true; // this ray hits the triangle 

}

void triangle::ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir, vec3& ColValue)
{

    vec3 lightToPt, surNorm, rVec, ttvec, ambCol, diffColour;
    float Cs, tt; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
    float vecdot;

    ambCol = currU * currColour + currV * currColour1 + (1 - currU - currV) * currColour2;

    //Calculate normal value and light ray
    lightToPt = normalize(sourcePt - IntPt);
    surNorm = normalize(currU * currNorm0 + currV * currNorm1 + (1 - currU - currV) * currNorm2);

    //Ambient light
    ambCol.r *= ambLightIntensity.r;
    ambCol.g *= ambLightIntensity.g;
    ambCol.b *= ambLightIntensity.b;;


    //Diffuse lighting calculation
    diffColour = ambCol * ambLightIntensity * std::max(0.0f, dot(lightToPt, surNorm));

    //compute specular value
    vecdot = dot(surNorm, lightToPt);
    ttvec.x = surNorm.x * 2.0 * vecdot;
    ttvec.y = surNorm.y * 2.0 * vecdot;
    ttvec.z = surNorm.z * 2.0 * vecdot;

    rVec = ttvec - lightToPt;
    tt = std::max(0.0, (double)dot(rVec, -dir));
    Cs = pow(tt, 20) * currSpecIntensity;

    //ColValue = Cs;
    ColValue = diffColour + Cs;

}

