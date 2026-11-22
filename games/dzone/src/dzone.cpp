#include "dzone.h"
#include "components.h"
#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/log.h"
#include "impala/physics.h"
#include "impala/sprite.h"
#include "systems/lifetime_system.h"
#include "systems/movement_system.h"
#include "systems/weapons_system.h"

namespace dzone {

    void DzoneGame::Init(impala::GameContext* context) {
        impala::log::Debug(impala::log::LogCategory::Game, "Initializing Dzone game");

        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas(dzone::KENNEY_SPACE_SHOOTER_SPRITE_ATLAS);

        _activeScene = context->sceneManager->CreateScene();
        auto& p1 = _players.AddPlayer("Player 1", context, _activeScene);
        p1.TeleportPositionAndRotation({200.0f, 200.0f}, 120.0f, _activeScene);

        auto& p2 = _players.AddPlayer("Player 2", context, _activeScene);
        p2.TeleportPositionAndRotation({1600.0f, 800.0f}, -45.0f, _activeScene);

        auto asteroidSprite =
            assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_METEOR_BROWN_BIG_1);
        assetDataBase->LoadSpriteTexture(asteroidSprite);
        auto m = _activeScene->CreateEntity("meteor", {600.0f, 0.0f}, 0.0f);
        auto& v = _activeScene->AddComponent<impala::Velocity2d>(m);
        v.velocity = {10.0f, 2.0f};
        v.angularVelocity = 30.0f;
        auto& renderer = _activeScene->AddComponent<impala::SpriteRenderer>(m);
        renderer.sprite = asteroidSprite;
    }

    void DzoneGame::FixedUpdate(impala::GameContext* context, float dt) {
        for (uint32_t i = 0; i < _players.PlayerCount(); ++i) {
            _players[i].UpdateControls(dt, _activeScene);
        }
        MovementSystem movementSystem;
        movementSystem.Update(dt, _activeScene, context);
        WeaponsSystem weaponsSystem;
        weaponsSystem.Update(dt, _activeScene, context);
        LifetimeSystem lifetimeSystem;
        lifetimeSystem.Update(_activeScene, dt);
    }

    void DzoneGame::OnCollisions(impala::IScene* scene,
                                 const std::span<impala::Collision2d>& collisions) {
        for (const auto& collision : collisions) {
            auto a = collision.a;
            auto b = collision.b;
            impala::log::Debug(impala::log::LogCategory::Game,
                               "Collision detected between entities {} and {}", a.id, b.id);
            auto destroyA = scene->GetComponent<DestroyOnCollision>(a);
            if (destroyA) {
                scene->DestroyEntity(a);
            }
            auto destroyB = scene->GetComponent<DestroyOnCollision>(b);
            if (destroyB) {
                scene->DestroyEntity(b);
            }
            auto healthA = scene->GetComponent<Health>(a);
            auto damageA = scene->GetComponent<Damage>(a);
            auto armorA = scene->GetComponent<Armor>(a);
            auto healthB = scene->GetComponent<Health>(b);
            auto damageB = scene->GetComponent<Damage>(b);
            auto armorB = scene->GetComponent<Armor>(b);
            if (healthA && damageB) {
                auto totalDamage = damageB->amount;
                if (armorA && armorA->current > 0.0f) {
                    auto armorDamage = totalDamage * (1.0f - armorA->current);
                    auto armorAbsorbed = std::min(armorA->current, armorDamage);
                    armorA->current -= armorAbsorbed;
                    totalDamage -= armorAbsorbed;
                }
                healthA->current -= totalDamage;
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Entity {} took damage: {} - Health: {}", a, damageB->amount,
                                   healthA->current);
                if (healthA->current <= 0) {
                    scene->DestroyEntity(a);
                }
            }
            if (healthB && damageA) {
                auto totalDamage = damageA->amount;
                if (armorB && armorB->current > 0.0f) {
                    auto armorDamage = totalDamage * (1.0f - armorB->current);
                    auto armorAbsorbed = std::min(armorB->current, armorDamage);
                    armorB->current -= armorAbsorbed;
                    totalDamage -= armorAbsorbed;
                }
                healthB->current -= totalDamage;
                impala::log::Debug(impala::log::LogCategory::Game,
                                   "Entity {} took damage: {} - Health: {}", b, damageA->amount,
                                   healthB->current);
                if (healthB->current <= 0) {
                    scene->DestroyEntity(b);
                }
            }
        }
    }

    void DzoneGame::Update(impala::GameContext* context, float dt) {}

    void DzoneGame::Render(impala::GameContext* context) {
        // Render game here
    }

    void DzoneGame::OnEvent(const impala::GameEvent& event) {
        // Handle game events here
    }

    void DzoneGame::OnGamepadConnected(impala::GamepadHandle handle) {
        impala::log::Debug(impala::log::LogCategory::Game, "Gamepad connected: {}", handle);
        _players.AddGamepadToPlayer(handle);
    }

    void DzoneGame::OnGamepadDisconnected(impala::GamepadHandle handle) {
        impala::log::Debug(impala::log::LogCategory::Game, "Gamepad disconnected: {}", handle);
        _players.RemoveGamepadFromPlayer(handle);
    }

    void DzoneGame::Shutdown() {
        // Cleanup code here
    }

} // namespace dzone
