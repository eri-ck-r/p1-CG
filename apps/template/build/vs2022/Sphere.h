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
		vec3f newPoint = worldToLocalMatrix().transform(ray.origin);
		vec3f newDirection = worldToLocalMatrix().transformVector(ray.direction);
		ray3f newRay{ newPoint, newDirection.versor() };

		float a = newRay.direction.dot(newRay.direction);
		float b = 2.0f * newRay.direction.dot(newRay.origin - _center);
		float c = (newRay.origin - _center).dot(newRay.origin - _center) - _radius * _radius;

		float delta = b * b - (4 * a * c);
		if (cg::math::isNegative(delta))
			return false;

		float t1 = (-1 * b + sqrt(delta)) / 2.0f * a;
		float t2 = (-1 * b - sqrt(delta)) / 2.0f * a;
		float tmin = std::numeric_limits<float>::max();

		if (cg::math::isPositive(t1))
			tmin = cg::math::min(t1, tmin);
		if (cg::math::isPositive(t2))
			tmin = cg::math::min(t2, tmin);

		t = tmin / newDirection.length();
		return ( (tmin != std::numeric_limits<float>::max()));

	}

	vec3f normalAt(const vec3f& p) const override
	{
		vec3f newPoint = worldToLocalMatrix().transform(p);
		return localToWorldMatrix().transformVector((newPoint - _center) * (1.0f / _radius)).versor();
	}
};
#endif // !__Sphere_h
