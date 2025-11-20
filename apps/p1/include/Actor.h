#ifndef __Actor3_h
#define __Actor3_h

#include "core/SharedObject.h"
#include "graphics/Material.h"

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
        Actor3(const Shape3& shape, const TriangleMesh& mesh) :
            _shape{&shape},
            _mesh{&mesh},
            _material{Material::defaultMaterial()}
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

        Shape3* shape()
        {
            return _shape;
        }

        Material* material()
        {
            return _material;
        }

        TriangleMesh* mesh()
        {
            return _mesh;
        }

        //mat4f& transform() { return _transform; };
    };

};

#endif