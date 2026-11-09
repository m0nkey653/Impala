#pragma once
#include "math.h"

namespace impala {

    struct Transform2d {
        math::Vec2 position;
        float rotation;
        math::Vec2 scale = {1.0f, 1.0f};
    };

    struct Transform3d {
        math::Vec3 position;
        math::Vec3 rotation;
        math::Vec3 scale = {1.0f, 1.0f, 1.0f};
    };

} // namespace impala