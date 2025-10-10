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
	Sphere(vec3f center, float radius) :
		_center(center.versor()), _radius(radius)
	{
		// do nothing
	}
	
	bool intersect(const ray3f& ray, float& t) const override
	{
		float a = ray.direction.dot(ray.direction);
		float b = 2.0f * ray.direction.dot(ray.origin - _center);
		float c = (ray.origin - _center).dot(ray.origin - _center) - _radius * _radius;

		float delta = b * b - (4 * a * c);
		if (cg::math::isNegative(delta))
			return false;

		float t1 = (-1 * b + sqrt(delta)) / 2.0f * a;
		float t2 = (-1 * b - sqrt(delta)) / 2.0f * a;

		if (t1 > 1e-6)
		{
			t = t1;
			return true;
		}

		if (t2 > 1e-6)
		{
			t = t2;
			return true;
		}

		return false;
	}
	

	//bool intersect(const ray3f& ray, float& t) const override
	//{
	//	/*auto oc = ray.origin - _center;
	//	float a = ray.direction.dot(ray.direction);
	//	float b = 2.0f * ray.direction.dot(ray.origin);
	//	float c = ray.origin.dot(ray.origin) - _radius * _radius;*/

	//	vec3f L = ray.origin - _center;
	//	float a = ray.direction.dot(ray.direction);
	//	float b = -2.0f * ray.direction.dot(L);
	//	float c = L.dot(L) - _radius * _radius;

	//	// delta = b^2-4ac
	//	float delta = b * b - (4 * a * c);
	//	if (cg::math::isNegative(delta))
	//		return false;

	//	float t0 = (-b + sqrt(delta)) / 2.0f * a;
	//	float t1 = (-b - sqrt(delta)) / 2.0f * a;

	//	if (t0 > t1) std::swap(t0, t1);

	//	if (t0 < 0) {
	//		t0 = t1; // If t0 is negative, let's use t1 instead.
	//		if (t0 < 0) return false; // Both t0 and t1 are negative.
	//	}

	//	t = t0;
	//	return true;
	//}

	vec3f normalAt(const vec3f& p) const override
	{
		if (cg::math::isEqual((_center - p).length(), _radius))
			return (p - _center) * (1.0f / _radius);
		else
			return {0, 0, 0};
	}
};
#endif // !__Sphere_h
