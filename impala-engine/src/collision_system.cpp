#include "collision_system.h"
#include "impala/log.h"
#include "impala/utility.h"
#include <cassert>
#include <variant>

namespace impala {

    bool CheckCollision(const WorldTransform2d& t1, const Collider& c1, const WorldTransform2d& t2,
                        const Collider& c2, Collision2d& collision) {

        float combinedRadius = c1.boundingRadius + c2.boundingRadius;
        if (combinedRadius * combinedRadius <
            math::LengthSquared(t2.transform.position - t1.transform.position)) {
            return false;
        }

        // TODO: Implement actual collision detection
        return true;
    }

    collision_system::WorldShape CollisionSystem::TransformCollider(const WorldTransform2d* wt,
                                                                    const ColliderShape& c) {
        collision_system::WorldShape ws;
        auto delta = impala::math::RotateDegrees(c.offset, wt->transform.rotation);
        ws =
            std::visit(impala::utility::Overloaded{[wt, delta](const CircleCollider& c) {
                                                       collision_system::WorldCircle wc;
                                                       wc.radius = c.radius;
                                                       wc.position = wt->transform.position + delta;
                                                       return collision_system::WorldShape{wc};
                                                   },
                                                   [wt, delta](const AABBCollider& a) {
                                                       collision_system::WorldBoxAABB wb;
                                                       wb.halfExtents = a.halfExtents;
                                                       wb.position = wt->transform.position + delta;
                                                       return collision_system::WorldShape{wb};
                                                   },
                                                   [wt, delta](const CapsuleCollider& cap) {
                                                       collision_system::WorldCapsule wcap;
                                                       wcap.radius = cap.radius;
                                                       wcap.halfLength = cap.halfLength;
                                                       wcap.rotation = cap.rotation;
                                                       wcap.position =
                                                           wt->transform.position + delta;
                                                       return collision_system::WorldShape{wcap};
                                                   }},
                       c.shape);
        return ws;
    }

    collision_system::CollisionEntry&
    CollisionSystem::GetCollisionEntry(int index, impala::ImpalaScene* scene, EntityHandle entity) {

        if (index < _collisionEntries.size()) {
            return _collisionEntries[index];
        }

        auto wt = scene->GetComponent<impala::WorldTransform2d>(entity);
        auto c = scene->GetComponent<impala::Collider>(entity);
        assert(wt && c && "Entity with Collider is missing Transform2d or Collider2d component");
        collision_system::CollisionEntry entry = {.handle = entity,
                                                  .layerIndex = c->layerIndex,
                                                  .mask = c->mask,
                                                  .boundingRadius = c->boundingRadius,
                                                  .shapeStart = _worldShapes.size(),
                                                  .shapeCount = c->colliderCount};

        for (auto i = 0; i < c->colliderCount; ++i) {
            _worldShapes.push_back(TransformCollider(wt, c->colliders[i]));
        }
        _collisionEntries.push_back(entry);

        return _collisionEntries.back();
    }

    std::span<Collision2d> CollisionSystem::CalculateCollisions(impala::ImpalaScene* scene) {
        auto entities =
            scene->GetEntitiesWithComponents<impala::WorldTransform2d, impala::Collider>();

        if (entities.empty()) {
            return std::span<Collision2d>();
        }

        _collisions.clear();
        for (auto i = 0; i < entities.size(); ++i) {
            auto& entry = GetCollisionEntry(i, scene, entities[i]);

            for (auto j = i + 1; j < entities.size(); ++j) {
                auto& otherEntry = GetCollisionEntry(j, scene, entities[j]);

                // skip if the collision layers do not match
                if (!collision_system::CanCollideWith(&entry, &otherEntry)) {
                    continue;
                }

                for (auto k = 0; k < entry.shapeCount; ++k) {
                    auto& shapeA = _worldShapes[entry.shapeStart + k];
                    for (auto l = 0; l < otherEntry.shapeCount; ++l) {
                        auto& shapeB = _worldShapes[otherEntry.shapeStart + l];
                        collision_system::Contact contact;
                        if (collision_system::OverlapShapes(shapeA, shapeB, contact)) {
                            Collision2d collision;
                            collision.a = entry.handle;
                            collision.b = otherEntry.handle;
                            collision.normal = contact.normal;
                            collision.depth = contact.depth;
                            _collisions.push_back(collision);
                            impala::log::Debug(impala::log::LogCategory::Engine,
                                               "Collision detected between entities {} and {}",
                                               entry.handle, otherEntry.handle);
                        }
                    }
                }
            }
        }

        return _collisions;
    }

    bool collision_system::Overlap(const collision_system::WorldCircle& a,
                                   const collision_system::WorldCircle& b,
                                   collision_system::Contact& out) {
        auto lengthSqr = impala::math::LengthSquared(b.position - a.position);
        auto radiusSum = a.radius + b.radius;
        if (lengthSqr > radiusSum * radiusSum) {
            return false;
        }
        auto length = std::sqrt(lengthSqr);
        out.normal = (b.position - a.position) / length;
        out.depth = radiusSum - length;
        return true;
    }
    bool collision_system::Overlap(const collision_system::WorldCircle& a,
                                   const collision_system::WorldBoxAABB& b,
                                   collision_system::Contact& out) {
        auto north = b.position.y - b.halfExtents.y;
        if (a.position.y + a.radius < north) {
            out.normal = {0, -1};
            out.depth = north - (a.position.y + a.radius);
            return true;
        }
        auto south = b.position.y + b.halfExtents.y;
        if (a.position.y - a.radius > south) {
            out.normal = {0, 1};
            out.depth = (a.position.y - a.radius) - south;
            return true;
        }
        auto east = b.position.x + b.halfExtents.x;
        if (a.position.x - a.radius > east) {
            out.normal = {1, 0};
            out.depth = (a.position.x - a.radius) - east;
            return true;
        }
        auto west = b.position.x - b.halfExtents.x;
        if (a.position.x + a.radius < west) {
            out.normal = {-1, 0};
            out.depth = west - (a.position.x + a.radius);
            return true;
        }
        return false;
    }
    bool collision_system::Overlap(const collision_system::WorldCircle& a,
                                   const collision_system::WorldCapsule& b,
                                   collision_system::Contact& out) {
        return false;
    }
    bool collision_system::Overlap(const collision_system::WorldBoxAABB& a,
                                   const collision_system::WorldBoxAABB& b,
                                   collision_system::Contact& out) {
        return false;
    }
    bool collision_system::Overlap(const collision_system::WorldBoxAABB& a,
                                   const collision_system::WorldCapsule& b,
                                   collision_system::Contact& out) {
        return false;
    }
    bool collision_system::Overlap(const collision_system::WorldCapsule& a,
                                   const collision_system::WorldCapsule& b,
                                   collision_system::Contact& out) {
        return false;
    }

} // namespace impala
