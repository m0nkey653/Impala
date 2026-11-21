#pragma once
#include "handles.h"
#include "impala/math.h"
#include "impala/utility.h"
#include <algorithm>
#include <array>
#include <cstdint>
#include <variant>

namespace impala {

    struct Collision2d {
        EntityHandle a;
        EntityHandle b;
        math::Vec2 normal{0, 0};
        float depth = 0.0f;
    };

    struct CircleCollider {
        float radius = 0;
    };

    // Axis-Aligned Bounding Box (AABB) Collider
    // only to be used with entities that don't rotate
    struct AABBCollider {
        impala::math::Vec2 halfExtents = {0, 0};
    };

    struct CapsuleCollider {
        // rotation of the capsule in degrees
        // 0 is vertical
        float rotation = 0;
        float radius = 0;
        float halfLength = 0;
    };

    struct ColliderShape {
        impala::math::Vec2 offset = {0, 0};
        std::variant<CircleCollider, AABBCollider, CapsuleCollider> shape;
    };

    struct Collider {
        std::array<ColliderShape, 8> colliders;
        uint8_t colliderCount = 0;
        float boundingRadius = 0;
        uint32_t layerIndex = 0;
        uint32_t mask = 0xFFFFFFFF; // collision mask, default to collide with all layers

        [[nodiscard]]
        bool AddShape(const ColliderShape& shape) {
            if (colliderCount >= colliders.size()) {
                return false;
            }
            colliders[colliderCount++] = shape;

            float shapeRadius = std::visit(
                impala::utility::Overloaded{
                    [](const CircleCollider& c) { return c.radius; },
                    [](const AABBCollider& a) { return impala::math::Length(a.halfExtents); },
                    [](const CapsuleCollider& cap) { return cap.radius + cap.halfLength; }},
                shape.shape);
            float offsetLength = impala::math::Length(shape.offset);
            boundingRadius = std::max(boundingRadius, offsetLength + shapeRadius);

            return true;
        }
    };
} // namespace impala
