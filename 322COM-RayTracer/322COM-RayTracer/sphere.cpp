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

void Sphere::ComputeColour(vec3 ambLightIntensity, float specIntensity, vec3 ambCol, vec3 diffColour, const vec3 sourcePt, const vec3 IntPt, const vec3 CenPt, const vec3 dir, vec3& ColValue)
{
	vec3 lightToPt, surNorm, rVec, ttvec;
	float Cs, tt; //Ca for ambient colour; //Cd for diffuse colour; //Cs for specular highlights
	float vecdot;

	//Calculate normal value and light ray
	lightToPt = normalize(sourcePt - IntPt);
	surNorm = normalize(IntPt - CenPt);

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
	Cs = pow(tt, 20) * specIntensity;

	//ColValue = Cs;
	ColValue = diffColour + Cs;
}
