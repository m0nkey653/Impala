#pragma once
#include "math.h"

namespace impala {

    struct Transform2d {
        // Position of the transform in 2D space
        math::Vec2 position{0, 0};
        // Rotation in degrees, clockwise, 0 corresponds to up (0, -1) direction
        // Do not set directly, use RotateDegrees or SetRotationDegrees functions instead
        float rotation = 0.0f;
        math::Vec2 scale = {1.0f, 1.0f};
    };

    // Rotates the transform by the specified angle in degrees clockwise
    // 0 degrees corresponds to up (0, -1) direction
    // This keeps the rotation normalized to [0, 360) degrees
    inline void RotateDegrees(Transform2d& transform2d, float degrees) {
        transform2d.rotation += degrees;
        if (transform2d.rotation < 0.0f || transform2d.rotation >= 360.0f) {
            transform2d.rotation = std::fmod(transform2d.rotation, 360.0f);
        }
    }

    // Sets the rotation of the transform to the specified angle in degrees clockwise
    // 0 degrees corresponds to up (0, -1) direction
    // This keeps the rotation normalized to [0, 360) degrees
    inline void SetRotationDegrees(Transform2d& transform2d, float degrees) {
        transform2d.rotation = degrees;
        if (transform2d.rotation < 0.0f || transform2d.rotation >= 360.0f) {
            transform2d.rotation = std::fmod(transform2d.rotation, 360.0f);
        }
    }

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
