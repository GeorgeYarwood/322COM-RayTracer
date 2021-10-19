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
