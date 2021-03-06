#include "shape.h"

shape::shape()
{
}

bool shape::intersection(vec3& orig, vec3& dir, rayHit& hit)
{
	return true;
}

vec3 shape::ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir)
{
	vec3 lightToPt, surNorm, rVec, ttvec, ambCol, diffColour;
	float Cs, tt; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float vecdot;

	ambCol = currColour;

	//Calculate normal value and light ray
	lightToPt = normalize(sourcePt - IntPt);
	surNorm = normalize(IntPt - currPos);

	//Ambient light
	ambCol.r *= ambLightIntensity.r;
	ambCol.g *= ambLightIntensity.g;
	ambCol.b *= ambLightIntensity.b;


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

	
	return diffColour + Cs;

	
	

}

