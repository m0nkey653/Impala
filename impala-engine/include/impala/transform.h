#pragma once
#include "math.h"

namespace impala {

    struct Transform2d {
        math::Vec2 position;
        float rotation;
        math::Vec2 scale;
    };

    struct Transform3d {
        math::Vec3 position;
        math::Vec3 rotation;
        math::Vec3 scale;
    };

} // namespace impala