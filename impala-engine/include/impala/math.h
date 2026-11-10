#pragma once

namespace impala::math {
    struct Vec2 {
        float x;
        float y;
    };

    constexpr Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
        return Vec2{lhs.x + rhs.x, lhs.y + rhs.y};
    }

    constexpr Vec2 operator+=(Vec2& lhs, const Vec2& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr float operator*(const Vec2& lhs, float rhs) {
        return lhs.x * rhs + lhs.y * rhs;
    }

    struct Vec3 {
        float x;
        float y;
        float z;
    };

    constexpr Vec3 operator+(const Vec3& lhs, const Vec3& rhs) {
        return Vec3{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
    }

    constexpr Vec3 operator+=(Vec3& lhs, const Vec3& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        lhs.z += rhs.z;
        return lhs;
    }

    constexpr float operator*(const Vec3& lhs, float rhs) {
        return lhs.x * rhs + lhs.y * rhs + lhs.z * rhs;
    }
} // namespace impala::math
