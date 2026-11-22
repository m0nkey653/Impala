#include "lifetime_system.h"
#include "../components.h"
#include <impala/log.h>
#include <impala/scene.h>

namespace dzone {

    void LifetimeSystem::Update(impala::IScene* scene, float deltaTime) {
        auto entities = scene->GetEntitiesWithComponents<Lifetime>();
        for (auto entity : entities) {
            auto lifetime = scene->GetComponent<Lifetime>(entity);

            lifetime->remaining -= deltaTime;
            if (lifetime->remaining <= 0.0f) {
                impala::log::Debug(impala::log::LogCategory::Engine,
                                   "Entity {} has expired and will be destroyed", entity);
                scene->DestroyEntity(entity);
            }
        }
    }

} // namespace dzone
