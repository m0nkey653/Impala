#include "movement_system.h"
#include "../components.h"
#include "impala/log.h"
#include "impala/math.h"
#include "impala/transform.h"

namespace dzone {

    void MovementSystem::Update(float deltaTime, impala::IScene* scene,
                                impala::GameContext* gameContext) {

        // update players
        std::vector<impala::EntityHandle> playerEntities =
            scene->GetEntitiesWithComponents<impala::Transform2d, Movement, ShipControls, Thrust>();

        for (auto entity : playerEntities) {
            // Process each entity here
            // impala::log::Debug(impala::log::LogCategory::Game, "Processing entity {}",
            // entity.id);
            auto transform = scene->GetComponent<impala::Transform2d>(entity);
            auto movement = scene->GetComponent<Movement>(entity);
            auto controls = scene->GetComponent<ShipControls>(entity);
            auto thrust = scene->GetComponent<Thrust>(entity);
            if (!thrust || !controls || !movement || !transform) {
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Skipping entity due to missing components {}", entity.id);
                continue;
            }

            movement->angularVelocity = controls->turn * thrust->turnRate;
            // impala::math::Vec2 heading = impala::Forward(*transform);
            movement->velocity =
                impala::Forward(*transform) * controls->throttle * thrust->maxSpeed;
            // Log the updated movement for debugging
            // impala::log::Debug(impala::log::LogCategory::Game,
            //                    "Entity {}: velocity = ({}, {}), angularVelocity = {}", entity.id,
            //                    movement->velocity.x, movement->velocity.y,
            //                    movement->angularVelocity);
        }

        std::vector<impala::EntityHandle> movementEntities =
            scene->GetEntitiesWithComponents<impala::Transform2d, Movement>();
        for (auto entity : movementEntities) {
            auto transform = scene->GetComponent<impala::Transform2d>(entity);
            auto movement = scene->GetComponent<Movement>(entity);

            transform->position += movement->velocity * deltaTime;
            transform->rotation += movement->angularVelocity * deltaTime;
        }
    }

} // namespace dzone
