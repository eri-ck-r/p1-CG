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
    protected:
      Reference<PrimitiveMapper> _mapper;
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

        void setShape(const Shape3& shape)
        {
            _shape = &shape;
        }

        auto shape()
        {
            return _shape;
        }

        Reference<Material>& material()
        {
            return _material;
        }

        auto& mesh()
        {
            return _mesh;
        }

        PrimitiveMapper* mapper() const
        {
          return _mapper;
        }

        //mat4f& transform() { return _transform; };
    };

};

#endif