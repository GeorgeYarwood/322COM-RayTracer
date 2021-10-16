#include "sphere.h"

Sphere::Sphere(vec3 pos, float rad, vec3 col, float diffIntensity, float specIntensity) {
	currPos = pos;
	currRad = rad;
	currColour = col;
	currDiffuseIntensity = diffIntensity;
	currSpecIntensity = specIntensity;
}

Sphere::~Sphere()
{
}

bool Sphere::intersection(vec3& orig, vec3& dir, rayHit& hit)
{
	float t0, t1;
	// geometric solution  // vector dir has to be normalize, length is 1.0
	vec3 L = currPos - orig;
	float tca = dot(L, dir);
	if (tca < 0) return false;
	float rayDir = dot(L, L) - tca * tca;
	if (rayDir > (currRad * currRad)) return false;

	float thc = sqrt(currRad * currRad - rayDir);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0) return false; // both t0 and t1 are negative 
	}

	hit.intersectPoint = orig + dir * t0;

	hit.rayDist = t0;
	return true;

}

//void Sphere::ComputeColour(vec3 ambLightIntensity, float specIntensity, vec3 ambCol, vec3 diffColour, const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, vec3& ColValue)
//{
//	
//}
