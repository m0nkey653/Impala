#include "lifetime_system.h"
#include "../components.h"
#include <impala/scene.h>

namespace dzone {

    void LifetimeSystem::Update(impala::IScene* scene, float deltaTime) {
        auto entities = scene->GetEntitiesWithComponents<Lifetime>();
        for (auto entity : entities) {
            auto lifetime = scene->GetComponent<Lifetime>(entity);

            lifetime->remaining -= deltaTime;
            if (lifetime->remaining <= 0.0f) {
                scene->DestroyEntity(entity);
            }
        }
    }

} // namespace dzone