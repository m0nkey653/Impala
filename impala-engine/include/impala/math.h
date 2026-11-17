#pragma once
#include <cmath>

namespace impala::math {

    constexpr float PI = 3.14159265358979323846f;
    constexpr float ZERO_COMPARISON = 1e-6f;

    constexpr float DEG2RAD = PI / 180.0f;
    constexpr float RAD2DEG = 180.0f / PI;

    constexpr float DegreesToRadians(float degrees) {
        return degrees * DEG2RAD;
    }

    constexpr float RadiansToDegrees(float radians) {
        return radians * RAD2DEG;
    }

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

    constexpr Vec2 operator-(const Vec2& lhs, const Vec2& rhs) {
        return Vec2{lhs.x - rhs.x, lhs.y - rhs.y};
    }

    constexpr Vec2 operator*(const Vec2& lhs, float rhs) {
        return Vec2{lhs.x * rhs, lhs.y * rhs};
    }

    constexpr Vec2 operator*(float lhs, const Vec2& rhs) {
        return Vec2{lhs * rhs.x, lhs * rhs.y};
    }

    constexpr Vec2 operator/(const Vec2& lhs, float rhs) {
        return Vec2{lhs.x / rhs, lhs.y / rhs};
    }

    inline float Length(const Vec2& v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    constexpr float LengthSquared(const Vec2& v) {
        return v.x * v.x + v.y * v.y;
    }

    // Rotates the vector 'v' by 'angle' radians clockwise
    // 0 radians corresponds to up (0, -1) direction
    inline Vec2 RotateRadians(const Vec2& v, float angle) {
        // positive angles rotate clockwise
        // sin and cos expect radians, so convert degrees to radians
        // sin(angle) = opposite / hypotenuse
        // cos(angle) = adjacent / hypotenuse
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        return Vec2{v.x * cosA - v.y * sinA, v.x * sinA + v.y * cosA};
    }

    // Rotates the vector 'v' by 'angle' degrees clockwise
    // 0 degrees corresponds to up (0, -1) direction
    inline Vec2 RotateDegrees(const Vec2& v, float angle) {
        return RotateRadians(v, DegreesToRadians(angle));
    }

    // Linearly interpolates between two angles, taking the shortest path around the circle
    inline float LerpAngle(float prevAngle, float currAngle, float alpha) {
        float diff = std::fmod(currAngle - prevAngle + 180.0f, 360.0f);
        if (diff < 0.0f)
            diff += 360.0f;
        diff -= 180.0f;
        return prevAngle + diff * alpha;
    }

    // Returns the normalized direction vector from 'from' to 'to'
    inline Vec2 Direction(const Vec2& from, const Vec2& to) {
        Vec2 d = to - from;
        float length = Length(d);
        if (length < ZERO_COMPARISON)
            return Vec2{0, 0};
        return Vec2{d.x / length, d.y / length};
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

    constexpr Vec3 operator-(const Vec3& lhs, const Vec3& rhs) {
        return Vec3{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }

    constexpr Vec3 operator-=(Vec3& lhs, const Vec3& rhs) {
        lhs.x -= rhs.x;
        lhs.y -= rhs.y;
        lhs.z -= rhs.z;
        return lhs;
    }

    constexpr Vec3 operator*(const Vec3& lhs, float rhs) {
        return Vec3{lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
    }

    constexpr Vec3 operator*(float lhs, const Vec3& rhs) {
        return Vec3{lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
    }
} // namespace impala::math
