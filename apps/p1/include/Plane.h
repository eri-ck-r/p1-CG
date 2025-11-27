#ifndef __Plane_h
#define __Plane_h

#include "Shape3.h"

class Plane : public Shape3
{
private:
  vec3f _p;
  vec3f _N;

public:
    Plane() :
        _p{ 0.0f, 0.0f, 0.0f },
        _N{ 0.0f, 1.0f, 0.0f }
    {
        // do nothing
    }

  bool intersect(const ray3f& ray, cg::Intersection& hit) const override
  {
    vec3f newOrigin = worldToLocalMatrix().transform(ray.origin);
    vec3f newDirection = worldToLocalMatrix().transformVector(ray.direction);
    ray3f newRay{ newOrigin, newDirection };
    float tLocal = -newRay.origin.y / (newRay.direction.y); //TODO simplifiacr isso aqui pq o plano tem origem no 0 0 0 e normal 0 1 0 entao fica -newOrigin.y/newDirection.y
    vec3f interPoint = newRay(tLocal);
    auto t = tLocal / newDirection.length();

    // retornar verdadeiro se -1 < x < 1 e -1 < z < 1
    if (cg::math::abs(interPoint.x) <= 1.0f && cg::math::abs(interPoint.z) <= 1.0f && cg::math::isPositive(t))
    {
      hit.distance = t;
      hit.object = this;
      return true;
    }

    return false;
  }

  vec3f normalAt(const vec3f& p) const override
  {
    return localToWorldMatrix().transformVector(_N).versor();
  }

  cg::Bounds3f bounds() const override
  {
    constexpr vec3f p0{ -1.0f, 0, -1.0f };
    constexpr vec3f p1{ 1.0f, 0.01, 1.0f };
    return cg::Bounds3f{ localToWorldMatrix().transform(p0), localToWorldMatrix().transform(p1) };
  }
};

#endif