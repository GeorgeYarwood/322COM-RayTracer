#include "sphere.h"

Sphere::Sphere(vec3 pos, float rad, vec3 col) {
	position = pos;
	radius = rad;
	mcolour = col;
}

Sphere::~Sphere()
{
}


bool Sphere::intersection(vec3 rayDirection, vec3 rayOrigin, float& t, vec3& IntPt, vec3& normVec)
{
	float t0, t1, tt; // solutions for t if the ray intersects 

	   // geometric solution  // vector dir has to be normalize, length is 1.0
	vec3 L = position - rayOrigin;
	float tca = dot(L, rayDirection);
	if (tca < 0) return false;

	float d = dot(L, L) - tca * tca;

	if (d > (radius * radius)) return false;

	float thc = sqrt(radius * radius - d);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) { tt = t1; t1 = t0; t0 = tt; }

	t = t0;

	IntPt.x = rayDirection.x * t;
	IntPt.y = rayDirection.y * t;
	IntPt.z = rayDirection.z * t;

	normVec = glm::normalize(IntPt - position);

	return true;

}

void Sphere::ComputeColour(const float ambientIntensity, const vec3 IntPt, const vec3 lightPt, const vec3 RayDirection, float& ColValue)
{
	vec3 lighttoPt, reflectLight, surNorm;
	float Ca, Cd, Cs, tt;

	lighttoPt = normalize(lightPt - IntPt);
	surNorm = normalize(IntPt - position);
	reflectLight = 2 * (dot(lighttoPt, surNorm)) * surNorm - lighttoPt;

	Ca = ambientIntensity;
	Cd = max(0.0, (double)dot(lighttoPt, surNorm));

	tt = max(0.0, (double)dot(reflectLight, -RayDirection));
	Cs = pow(tt, 50) * 0.7;

	ColValue = Ca + Cd + Cs;


	
}


