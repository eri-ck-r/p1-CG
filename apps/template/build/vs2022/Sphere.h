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
		float tmin = std::numeric_limits<float>::max();

		if (!cg::math::isNegative(t1))
		{
			tmin = cg::math::min(t1, tmin);
			if (!cg::math::isNegative(t2))
				tmin = cg::math::min(t2, tmin);
			t = tmin;
			return true;
		}

		return false;
	}

	vec3f normalAt(const vec3f& p) const override
	{
			return (p - _center) * (1.0f / _radius);
	}
};
#endif // !__Sphere_h
