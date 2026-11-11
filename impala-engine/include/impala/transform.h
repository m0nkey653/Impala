#pragma once
#include "math.h"

namespace impala {

    struct Transform2d {
        math::Vec2 position;
        float rotation = 0;
        math::Vec2 scale = {1.0f, 1.0f};
    };

    inline math::Vec2 Forward(const Transform2d& transform2d) {
        return math::RotateDegrees({0.0f, -1.0f}, transform2d.rotation);
    }

    inline math::Vec2 Back(const Transform2d& transform2d) {
        return math::RotateDegrees({0.0f, 1.0f}, transform2d.rotation);
    }
    inline math::Vec2 Left(const Transform2d& transform2d) {
        return math::RotateDegrees({-1.0f, 0.0f}, transform2d.rotation);
    }

    inline math::Vec2 Right(const Transform2d& transform2d) {
        return math::RotateDegrees({1.0f, 0.0f}, transform2d.rotation);
    }

    struct Transform3d {
        math::Vec3 position;
        math::Vec3 rotation;
        math::Vec3 scale = {1.0f, 1.0f, 1.0f};
    };

} // namespace impala
