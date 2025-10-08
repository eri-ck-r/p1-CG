#ifndef __Plane_h
#define __Plane_h

#include "Shape3.h"

class Plane : public Shape3
{
private:
    vec3f _p;
    vec3f _N;

public:
    Plane(vec3f p, vec3f N) : _p{ p }, _N{ N.versor()}
    {
        // do nothing
    }

    bool intersect(const ray3f& ray, float& t) const override
    {
        t = (_p - ray.origin).dot(_N) / (ray.direction.versor().dot(_N));

        return t > 0;
    }

    vec3f normalAt(const vec3f& p) const override
    {
        return _N;
    }
};

#endif