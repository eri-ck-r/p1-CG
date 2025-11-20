#ifndef __Scene_h
#define __Scene_h

#include "core/SharedObject.h"
#include "core/List.h"
#include "graphics/Light.h"
#include "graphics/Color.h"

#include "Actor.h"

namespace cg
{
    struct Scene : public SharedObject
    {
        List<Reference<Actor3>> actors;
        List<Reference<Light>> lights;
        Color backgroundColor;
        Color ambientLight;
    };

};

#endif