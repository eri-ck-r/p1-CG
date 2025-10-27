#ifndef __Actor_h
#define __Actor_h

#include "core/SharedObject.h"
#include "graphics/Material.h"

#include "Shape3.h"

namespace cg {

    class Actor : public SharedObject
    {
    private:
        Reference<Shape3> _shape;
        Reference<Material> _material;
        //mat4f _transform;
        //mat3f _normal;

    public:
        Actor(const Shape3& shape) : _shape{&shape}, _material{Material::defaultMaterial()} {}

       /* void setTransform(const mat4f& transform)
        {
            _transform = transform;
            _normal = mat3f{ transform };
        }

        }*/

        void setMaterial(const Material& material)
        {
            _material = &material;
        }

        Shape3* shape() { return _shape; }

        Material* material() const { return _material; }

        //mat4f& transform() { return _transform; };
    };

};

#endif