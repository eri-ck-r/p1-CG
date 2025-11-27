#ifndef __Actor3_h
#define __Actor3_h

#include "core/SharedObject.h"
#include "graphics/Material.h"
#include "graphics/PrimitiveMapper.h"

#include "Shape3.h"

namespace cg {

    class Actor3 : public SharedObject
    {
    private:
        Reference<Shape3> _shape;
        Reference<Material> _material;
        Reference<TriangleMesh> _mesh;
        mat4f _transform;
        mat3f _normal;
    public:
        float rugosity;
        float metalFactor;

        Actor3(const Shape3& shape, const TriangleMesh& mesh) :
            _shape{ &shape },
            _mesh{ &mesh },
            _material{ Material::defaultMaterial() },
            rugosity{ 0.5f },
            metalFactor{ 0.5f }
        {
            // do nothing
        }

        /*void setTransform(const mat4f& transform)
        {
            _transform = transform;
            _normal = mat3f{ transform };
        }
        */

        void setMaterial(const Material& material)
        {
            _material = &material;
        }

        auto& shape()
        {
            return _shape;
        }

        Material* material()
        {
            return _material;
        }

        auto& mesh()
        {
            return _mesh;
        }

        bool intersect(const ray3f& ray) const
        {
          cg::Intersection hit;
          return intersect(ray, hit);
        }

        bool intersect(const ray3f& ray, cg::Intersection& hit) const
        {
          if (_shape->intersect(ray, hit))
          {
            hit.object = this;
            return true;
          }
          return false;
        }

        cg::Bounds3f bounds() const
        {
          return _shape->bounds();
        }

        //mat4f& transform() { return _transform; };
    };

};

#endif