#include "movement_system.h"
#include "../components.h"
#include "impala/log.h"
#include "impala/math.h"
#include "impala/physics.h"
#include "impala/transform.h"

namespace dzone {

    void MovementSystem::Update(float deltaTime, impala::IScene* scene,
                                impala::GameContext* gameContext) {

        // update players
        std::vector<impala::EntityHandle> playerEntities =
            scene->GetEntitiesWithComponents<impala::Transform2d, impala::Velocity2d, ShipControls,
                                             Thrust>();

        for (auto entity : playerEntities) {
            auto transform = scene->GetComponent<impala::Transform2d>(entity);
            auto movement = scene->GetComponent<impala::Velocity2d>(entity);
            auto controls = scene->GetComponent<ShipControls>(entity);
            auto thrust = scene->GetComponent<Thrust>(entity);
            if (!thrust || !controls || !movement || !transform) {
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Skipping entity due to missing components {}", entity.id);
                continue;
            }

            movement->angularVelocity = controls->turn * thrust->turnRate;
            movement->velocity =
                impala::Forward(*transform) * controls->throttle * thrust->maxSpeed;
        }
    }

} // namespace dzone
