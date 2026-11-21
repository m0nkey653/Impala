#include "physics_system.h"
#include "cassert"
#include "impala/physics.h"
#include "impala/transform.h"
#include <concepts>

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

} // namespace impala
