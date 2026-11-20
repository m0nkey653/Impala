#include "physics_system.h"
#include "cassert"
#include "impala/physics.h"
#include "impala/transform.h"

namespace impala {

    void PhysicsSystem::ApplyVelocity(impala::ImpalaScene* scene, float deltaTime) {
        auto entities = scene->GetEntitiesWithComponents<impala::Transform2d, impala::Velocity2d>();
        for (const auto& entity : entities) {
            auto t = scene->GetComponent<impala::Transform2d>(entity);
            auto v = scene->GetComponent<impala::Velocity2d>(entity);
            assert(t && v && "Entity with Velocity is missing Transform2d or Velocity2d component");
            t->position += v->velocity * deltaTime;
            impala::RotateDegrees(*t, v->angularVelocity * deltaTime);
        }
    }

    bool CheckCollision(const Transform2d& t1, const Collider& c1, const Transform2d& t2,
                        const Collider& c2, Collision& collision) {
        float combinedRadius = c1.boundingRadius + c2.boundingRadius;
        if (combinedRadius * combinedRadius < math::LengthSquared(t2.position - t1.position)) {
            return false;
        }

        // TODO: Implement actual collision detection
        return true;
    }

    std::span<Collision> PhysicsSystem::CalculateCollisions(impala::ImpalaScene* scene) {
        _collisions.clear();
        for (const auto& entity :
             scene->GetEntitiesWithComponents<impala::Transform2d, impala::Collider>()) {
            auto t = scene->GetComponent<impala::Transform2d>(entity);
            auto c = scene->GetComponent<impala::Collider>(entity);
            assert(t && c && "Entity with Collider is missing Transform2d or Collider2d component");
            for (const auto& otherEntity :
                 scene->GetEntitiesWithComponents<impala::Transform2d, impala::Collider>()) {
                if (entity.id >= otherEntity.id)
                    continue;

                auto ot = scene->GetComponent<impala::Transform2d>(otherEntity);
                auto oc = scene->GetComponent<impala::Collider>(otherEntity);
                assert(ot && oc &&
                       "Entity with Collider is missing Transform2d or Collider2d component");

                Collision collision;
                if (impala::CheckCollision(*t, *c, *ot, *oc, collision)) {
                    _collisions.push_back(collision);
                }
            }
        }

        return _collisions;
    }
} // namespace impala
