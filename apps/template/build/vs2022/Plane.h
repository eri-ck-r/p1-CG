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
        vec3f newOrigin = worldToLocalMatrix().transform(ray.origin);
        vec3f newDirection = worldToLocalMatrix().transform(ray.direction).versor();
        ray3f newRay{ newOrigin, newDirection };
        t = (_p - newRay.origin).dot(_N) / (newRay.direction.dot(_N)); //todo simplifiacr isso aqui pq o plano tem origem no 0 0 0 e normal 0 1 0 entao fica -newOrigin.y/newDirection.y

        return t > 0;
    }

    vec3f normalAt(const vec3f& p) const override
    {
        return localToWorldMatrix().transformVector(_N);
    }
};

#endif