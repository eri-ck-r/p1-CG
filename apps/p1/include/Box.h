#ifndef BOX_H
#define BOX_H


#include "geometry/Bounds3.h"
#include "math/Vector3.h"
#include "geometry/Ray.h"
#include <algorithm>      
#include <limits>         
#include <cmath>          
#include "Shape3.h"
#include "geometry/Intersection.h" 


using namespace cg;

class Box : public Shape3
{
private:
  vec3f _pMin;
  vec3f _pMax;

public:

  Box(const vec3f& p1, const vec3f& p2)
  {
    for (int i = 0; i < 3; ++i) {
      _pMin[i] = std::min(p1[i], p2[i]);
      _pMax[i] = std::max(p1[i], p2[i]);
    }
  }

  bool intersect(const ray3f& ray, cg::Intersection& hit) const override
  {
    auto pMin = worldToLocalMatrix().transform(_pMin);
    auto pMax = worldToLocalMatrix().transform(_pMax);
    float t_min_box = -std::numeric_limits<float>::infinity();
    float t_max_box = std::numeric_limits<float>::infinity();

    for (int i = 0; i < 3; ++i)
    {
      float invDir = 1.0f / ray.direction[i];
      float t1 = (pMin[i] - ray.origin[i]) * invDir;
      float t2 = (pMax[i] - ray.origin[i]) * invDir;

      if (t1 > t2) {
        std::swap(t1, t2);
      }

      t_min_box = std::max(t_min_box, t1);
      t_max_box = std::min(t_max_box, t2);

      if (t_min_box > t_max_box)
        return false;
    }

    float final_t;
    if (t_min_box < 0) {
      if (t_max_box < 0) return false;
      final_t = t_max_box;
    }
    else {
      final_t = t_min_box;
    }

    hit.distance = final_t;
    hit.object = this;

    return true;
  }


  vec3f normalAt(const vec3f& p) const override
  {
    vec3f center = (_pMin + _pMax) * 0.5f;
    vec3f dimensions = _pMax - _pMin;
    vec3f p_local = p - center;

    vec3f half_dimensions = dimensions * 0.5f;

    vec3f normalized_p(
      (half_dimensions[0] != 0.0f) ? p_local[0] / half_dimensions[0] : 0.0f,
      (half_dimensions[1] != 0.0f) ? p_local[1] / half_dimensions[1] : 0.0f,
      (half_dimensions[2] != 0.0f) ? p_local[2] / half_dimensions[2] : 0.0f
    );

    int axis = 0;
    float max_abs_component = 0.0f;

    for (int i = 0; i < 3; ++i) {
      if (std::abs(normalized_p[i]) > std::abs(max_abs_component)) {
        max_abs_component = normalized_p[i];
        axis = i;
      }
    }

    vec3f normal(0.0f);

    if (max_abs_component > 0) {
      normal[axis] = 1.0f;
    }
    else {
      normal[axis] = -1.0f;
    }

    return normal;
  }


  cg::Bounds3f bounds() const override
  {
    return cg::Bounds3f(localToWorldMatrix().transform(_pMin), localToWorldMatrix().transform(_pMax));
  }
};

#endif // BOX_H