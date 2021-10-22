
#include "triangle.h"

triangle::triangle(vec3 pos, vec3 v0, vec3 v1, vec3 v2, vec3 col, vec3 col1,vec3 col2, vec3 norm0, vec3 norm1, vec3 norm2,float diffIntensity, float specIntensity)
{
    currPos = pos;

    currV0 = v0+ currPos;
    currV1 = v1+ currPos;
    currV2 = v2+ currPos;
   

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
    float t;
    vec3 v0v1 = currV1 - currV0;
    vec3 v0v2 = currV2 - currV0;
    vec3 pvec = cross(dir, v0v2);
    float det = dot(v0v1, pvec);
    
    if (det < 0.00001) return false;

    // ray and triangle are parallel if det is close to 0
    //if (abs(det) < 0.00001) return false;

    float invDet = 1 / det;

    vec3 tvec = orig - currV0;
    currU = dot(tvec,(pvec) * invDet);
    if (currU < 0 || currU > 1) return false;

    vec3 qvec = cross(tvec,v0v1);
    currV = dot(dir,(qvec) * invDet);
    if (currV < 0 || currU + currV > 1) return false;

    t= dot(v0v2, (qvec) * invDet);


    hit.intersectPoint = orig + dir * t;
    hit.rayDist = t;
    return true;


}

vec3 triangle::ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir)
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
    return diffColour + Cs;

}

