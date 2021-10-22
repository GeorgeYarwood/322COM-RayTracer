#include "AABB.h"

AABB::AABB(glm::vec3 min, glm::vec3 max, shape &shape)
{
    pMin = min;
    pMax = max;
	currShape = &shape;	
}


bool AABB::intersection(vec3& orig, vec3& dir, rayHit& hit)
{
	float epsilon = 0.0001f;

	float tmin = (pMin.x - orig.x) / (dir.x + epsilon);
	float tmax = (pMax.x - orig.x) / (dir.x + epsilon);

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (pMin.y - orig.y) / (dir.y + epsilon);
	float tymax = (pMax.y - orig.y) / (dir.y + epsilon);

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (pMin.z - orig.z) / (dir.z + epsilon);
	float tzmax = (pMax.z - orig.z) / (dir.z + epsilon);

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	return true;
}
