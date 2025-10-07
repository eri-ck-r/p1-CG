#ifndef __Plane_h
#define __Plane_h

#include "Shape3.h"

class Plane : public Shape3
{
private:
    vec3f p;
    vec3f N;

public:
    bool intersect(const ray3f& ray, float& t) const override
    {
        t = -((ray.origin - p).dot(N)) / (ray.direction.dot(N));

        return t > 0;
    }

    vec3f normalAt(const vec3f& p) const override
    {
        return N;
    }
};

#endif