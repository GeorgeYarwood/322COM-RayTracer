#include "plane.h"

plane::plane(vec3 col, vec3 point, vec3 normal)
{
	currColour = col;
	currPoint = point;
	currNormal = normal;
}

bool plane::intersection(vec3& orig, vec3& dir, rayHit& hit)
{

	normalize(orig);
	normalize(currPoint);
	normalize(currNormal);

	float denominator = dot(currNormal, dir);
	abs(denominator);
	if(denominator > 1e-6)
	{
		vec3 pointtoray = currPoint - orig;

		hit.rayDist = dot(pointtoray, dir) / denominator;
		//hit.rayDist = 12;
		
		
		hit.intersectPoint = orig + dir * hit.rayDist;

		 
		return(hit.rayDist >=0);
	}


	return false;
}
