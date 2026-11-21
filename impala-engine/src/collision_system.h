#pragma once
#include "impala/colliders.h"
#include "impala/handles.h"
#include "impala/math.h"
#include "impala_scene.h"
#include <span>
#include <variant>
#include <vector>

namespace impala {

    namespace collision_system {

        struct CollisionEntry {
            EntityHandle handle;
            uint32_t layerIndex = 0;
            uint32_t mask = 0;
            float boundingRadius = 0;
            size_t shapeStart = 0;
            size_t shapeCount = 0;
        };

        // Contact info for one shape pair. Normal points from a toward b.
        struct Contact {
            math::Vec2 normal{0, 0};
            float depth = 0.0f;
        };

        struct WorldCircle {
            math::Vec2 position{0, 0};
            float radius = 0;
        };

        struct WorldBoxAABB {
            math::Vec2 position{0, 0};
            math::Vec2 halfExtents{0, 0};
        };

        struct WorldCapsule {
            math::Vec2 position{0, 0};
            float rotation = 0; // in degrees
            float radius = 0;
            float halfLength = 0;
        };

        struct WorldShape {
            std::variant<WorldCircle, WorldBoxAABB, WorldCapsule> shape;
        };

        // The six unique tests, on world-space shapes. Declare these before the template.
        bool Overlap(const WorldCircle& a, const WorldCircle& b, Contact& out);
        bool Overlap(const WorldCircle& a, const WorldBoxAABB& b, Contact& out);
        bool Overlap(const WorldCircle& a, const WorldCapsule& b, Contact& out);
        bool Overlap(const WorldBoxAABB& a, const WorldBoxAABB& b, Contact& out);
        bool Overlap(const WorldBoxAABB& a, const WorldCapsule& b, Contact& out);
        bool Overlap(const WorldCapsule& a, const WorldCapsule& b, Contact& out);

        template <typename A, typename B>
        concept HasOverlap = requires(const A& a, const B& b, Contact& out) {
            { Overlap(a, b, out) } -> std::same_as<bool>;
        };

        inline bool OverlapShapes(const WorldShape& a, const WorldShape& b, Contact& out) {
            return std::visit(
                [&out](const auto& sa, const auto& sb) -> bool {
                    using A = std::decay_t<decltype(sa)>;
                    using B = std::decay_t<decltype(sb)>;
                    if constexpr (HasOverlap<A, B>) {
                        return Overlap(sa, sb, out);
                    } else {
                        static_assert(HasOverlap<B, A>, "No Overlap() for this shape pair");
                        if (!Overlap(sb, sa, out))
                            return false;
                        out.normal = out.normal * -1.0f; // flip so it still points a -> b
                        return true;
                    }
                },
                a.shape, b.shape);
        }
        constexpr bool CanCollideWith(const CollisionEntry* a, const CollisionEntry* b) {
            if (b->layerIndex >= 32 || a->layerIndex >= 32) {
                return false;
            }
            return (a->mask & (1u << b->layerIndex)) != 0 && (b->mask & (1u << a->layerIndex)) != 0;
        }
    } // namespace collision_system

    struct CollisionSystem {
    public:
        CollisionSystem() {
            _collisions.reserve(64);
            _collisionEntries.reserve(64);
            _worldShapes.reserve(128);
        };
        std::span<Collision2d> CalculateCollisions(impala::ImpalaScene* scene);

    private:
        collision_system::CollisionEntry& GetCollisionEntry(int index, impala::ImpalaScene* scene,
                                                            EntityHandle entity);
        static collision_system::WorldShape TransformCollider(const WorldTransform2d* wt,
                                                              const ColliderShape& c);

        // collisions vector for storing detected collisions
        // cleared on every frame
        std::vector<Collision2d> _collisions;
        // transformed world shapes for all colliders
        // cleared on every frame
        std::vector<collision_system::CollisionEntry> _collisionEntries;
        // every entity's collision entries, back to back
        // cleared on every frame
        std::vector<collision_system::WorldShape>
            _worldShapes; // every entity's world shapes, back to back
    };
} // namespace impala
