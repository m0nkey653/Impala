#pragma once
#include <cmath>

namespace impala::math {
    struct Vec2 {
        float x = 0;
        float y = 0;
    };

    constexpr Vec2 operator+(const Vec2& lhs, const Vec2& rhs) {
        return Vec2{lhs.x + rhs.x, lhs.y + rhs.y};
    }

    constexpr Vec2 operator+=(Vec2& lhs, const Vec2& rhs) {
        lhs.x += rhs.x;
        lhs.y += rhs.y;
        return lhs;
    }

    constexpr Vec2 operator*(const Vec2& lhs, float rhs) {
        return Vec2{lhs.x * rhs, lhs.y * rhs};
    }

    inline Vec2 RotateDegrees(const Vec2& v, float angle) {
        // sin and cos expect radians, so convert degrees to radians
        // sin(angle) = opposite / hypotenuse
        // cos(angle) = adjacent / hypotenuse
        float rad = angle * (3.14159265358979323846f / 180.0f);
        float cosA = std::cos(rad);
        float sinA = std::sin(rad);
        return Vec2{v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA};
    }

    struct Vec3 {
        float x = 0;
        float y = 0;
        float z = 0;
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
