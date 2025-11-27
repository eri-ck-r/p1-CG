#ifndef __Shape3_h
#define __Shape3_h

#include "core/SharedObject.h"
#include "geometry/Ray.h"
#include "math/Vector3.h"
#include "graphics/TransformableObject.h"
#include "geometry/Bounds3.h"

using ray3f = cg::Ray<float, 3>;
using vec3f = cg::Vector<float, 3>;

class Shape3 : public cg::TransformableObject
{
public:
	virtual bool intersect(const ray3f& ray, cg::Intersection& hit) const = 0;

	bool intersect(const ray3f& ray) const
	{
		cg::Intersection hit;
		return intersect(ray, hit);
	}

	virtual vec3f normalAt(const vec3f& p) const = 0;

	virtual cg::Bounds3f bounds() const = 0;

};


#endif // __Shape3_h