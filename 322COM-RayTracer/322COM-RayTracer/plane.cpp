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
	if(denominator > 1e-6)
	{
		int t;
		vec3 pointtoray = currPoint - orig;

		t = dot(pointtoray, currNormal) / denominator;

		hit.intersectPoint = orig + dir * hit.rayDist;

		hit.rayDist = t;
		return(t >= 0);
	}


	return false;
}
