#ifndef __Shape3_h
#define __Shape3_h

#include "core/SharedObject.h"
#include "geometry/Ray.h"
#include "math/Vector3.h"

using ray3f = cg::Ray<float, 3>;
using vec3f = cg::Vector<float, 3>;

class Shape3 : public cg::SharedObject
{
public:
	virtual bool intersect(const ray3f& ray, float& t) const = 0;

	virtual vec3f normalAt(const vec3f& p) const = 0;
};


#endif // __Shape3_h