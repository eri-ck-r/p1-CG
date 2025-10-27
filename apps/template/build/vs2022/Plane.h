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
        vec3f newDirection = worldToLocalMatrix().transformVector(ray.direction);
        ray3f newRay{ newOrigin, newDirection };
        float tLocal = -newRay.origin.y / (newRay.direction.y); //TODO simplifiacr isso aqui pq o plano tem origem no 0 0 0 e normal 0 1 0 entao fica -newOrigin.y/newDirection.y
        vec3f interPoint = newRay(tLocal);
        t = tLocal / newDirection.length();
        // retornar verdadeiro se -1 < x < 1 e -1 < z < 1
        return cg::math::abs(interPoint.x) <= 1.0f && cg::math::abs(interPoint.z) <= 1.0f && cg::math::isPositive(t);
    }

    vec3f normalAt(const vec3f& p) const override
    {
        return localToWorldMatrix().transformVector(_N).versor();
    }
};

#endif