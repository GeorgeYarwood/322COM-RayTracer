#include "plane.h"

plane::plane(vec3 col, vec3 point, vec3 normal)
{
	currColour = col;
	currPoint = point;
	currNormal = normal;
}

bool plane::intersection(vec3& orig, vec3& dir, rayHit& hit)
{
	float denominator = dot(currNormal, dir);
	if(abs(denominator) > 0.0001)
	{
		vec3 pointtoray = currPoint - orig;

		hit.rayDist = dot(pointtoray, currNormal) / (denominator);
		
		//hit.rayDist = 12;
		
		
		hit.intersectPoint.x = orig.x + dir.x * hit.rayDist;
		hit.intersectPoint.y = orig.y + dir.y * hit.rayDist;
		hit.intersectPoint.z = orig.z + dir.z * hit.rayDist;
		 
		return(hit.rayDist >=0);
	}


	return false;
}
void plane::ComputeColour(vec3 ambLightIntensity, const vec3 sourcePt, const vec3 IntPt, const vec3 dir, vec3& ColValue)
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
	ambCol.b *= ambLightIntensity.b;;


	//Diffuse lighting calculation
	diffColour = ambCol * ambLightIntensity * std::max(0.0f, dot(lightToPt, surNorm));

	

	//ColValue = Cs;
	ColValue = diffColour ;
}