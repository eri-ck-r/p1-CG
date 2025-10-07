#ifndef __Sphere_h
#define __Sphere_h

#include "Shape3.h"
#include "math/Real.h"
class Sphere : public Shape3
{
private:
	vec3f _center;
	float _radius;
public:
	bool intersect(const ray3f& ray, float& t) const override
	{
		float a = ray.direction.dot(ray.direction);
		float b = 2 * ray.direction.dot(ray.origin);
		float c = ray.origin.dot(ray.origin) - _radius * _radius;
		// delta = b^2-4ac
		float delta = b * b - 4 * a * c;
		if (delta < 0)
			return false;

		float t1 = (-1 * b + sqrt(delta)) / 2 * a;
		float t2 = (-1 * b - sqrt(delta)) / 2 * a;
		t = cg::math::max(t1, t2);
		return true;
	}

	vec3f normalAt(const vec3f& p) const override
	{
		if (cg::math::isEqual((_center - p).length(), _radius))
			return (p - _center) * (1.0f / _radius);
		else
			return {0, 0, 0};
	}
};
#endif // !__Sphere_h
