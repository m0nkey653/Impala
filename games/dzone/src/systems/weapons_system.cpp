#include "weapons_system.h"
#include "../components.h"
#include "impala/log.h"
#include "impala/sprite.h"
#include "impala/transform.h"

namespace dzone {

    void WeaponsSystem::Update(float deltaTime, impala::IScene* scene,
                               impala::GameContext* gameContext) {
        auto entities =
            scene->GetEntitiesWithComponents<impala::Transform2d, ShipControls, Weapons>();
        for (auto entity : entities) {
            auto shipControls = scene->GetComponent<ShipControls>(entity);
            auto weapons = scene->GetComponent<Weapons>(entity);
            auto transform = scene->GetComponent<impala::Transform2d>(entity);
            if (!shipControls || !weapons || !transform) {
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Skipping entity due to missing components {}", entity.id);
                continue;
            }

            if (weapons->activePrimary >= weapons->slots.size()) {
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Invalid active primary weapon for entity {}", entity.id);
                continue;
            }

            auto& weapon = weapons->slots[weapons->activePrimary];
            if (weapon.cooldownRemaining > 0.0f) {
                weapon.cooldownRemaining -= deltaTime;
            } else if (shipControls->firePrimary) {
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Firing primary weapon for entity {} with projectile {}",
                                   entity.id, weapon.projectile.name);

                weapon.cooldownRemaining = weapon.projectile.cooldown;
                auto e = scene->CreateEntity(weapon.projectile.name);
                scene->AddComponent<impala::Transform2d>(
                    e, {transform->position, transform->rotation});
                auto& m = scene->AddComponent<Movement>(e);
                m.velocity = impala::Forward(*transform) * weapon.projectile.speed;
                auto& s = scene->AddComponent<impala::SpriteRenderer>(e);
                s.sprite = gameContext->assetDataBase->GetSprite(weapon.projectile.spriteName);
            }
        }
    }
} // namespace dzone
