#include "physics_system.h"
#include "cassert"
#include "impala/physics.h"

namespace impala {

    void PhysicsSystem::SavePreviousTransforms(impala::ImpalaScene* scene) {
        auto entities = scene->GetEntitiesWithComponents<impala::Transform2d, impala::Velocity2d>();
        for (const auto& entity : entities) {
            auto t = scene->GetComponent<impala::Transform2d>(entity);
            assert(t && "Entity with Velocity is missing Transform2d component");
            auto prev = scene->GetComponent<impala::PreviousTransform2d>(entity);
            if (!prev) {
                scene->AddComponent<impala::PreviousTransform2d>(entity, {.transform = *t});
            } else {
                prev->transform = *t;
            }
        }
    }

    void PhysicsSystem::ApplyVelocity(impala::ImpalaScene* scene, float deltaTime) {
        auto entities = scene->GetEntitiesWithComponents<impala::Transform2d, impala::Velocity2d>();
        for (const auto& entity : entities) {
            auto t = scene->GetComponent<impala::Transform2d>(entity);
            auto v = scene->GetComponent<impala::Velocity2d>(entity);
            assert(t && v && "Entity with Velocity is missing Transform2d or Velocity2d component");
            t->position += v->velocity * deltaTime;
            t->rotation += v->angularVelocity * deltaTime;
        }
    }
} // namespace impala
