#include "dzone.h"
#include "components.h"
#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/log.h"
#include "impala/sprite.h"
#include "impala/transform.h"
#include "systems/movement_system.h"

namespace dzone {

    void DzoneGame::Init(impala::GameContext* context) {
        impala::log::Debug(impala::log::LogCategory::Game, "Initializing Dzone game");

        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas(dzone::KENNEY_SPACE_SHOOTER_SPRITE_ATLAS);
        auto playerSprite0 =
            assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_PLAYER_SHIP_1_BLUE);
        auto playerSprite1 =
            assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_PLAYER_SHIP_2_GREEN);
        auto asteroidSprite =
            assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_METEOR_BROWN_BIG_1);
        assetDataBase->LoadSpriteTexture(playerSprite0);
        assetDataBase->LoadSpriteTexture(playerSprite1);
        assetDataBase->LoadSpriteTexture(asteroidSprite);

        _activeScene = context->sceneManager->CreateScene();
        auto& p1 = _players.AddPlayer("Player 1", context, _activeScene, playerSprite0);
        p1.SetPosition({200.0f, 200.0f}, _activeScene);
        p1.SetRotation(0.0f, _activeScene);

        auto& p2 = _players.AddPlayer("Player 2", context, _activeScene, playerSprite1);
        p2.SetPosition({400.0f, 200.0f}, _activeScene);
        p2.SetRotation(0.0f, _activeScene);

        auto m = _activeScene->CreateEntity("meteor");
        auto& t = _activeScene->AddComponent<impala::Transform2d>(m);
        t.position = {0.0f, 0.0f};
        t.rotation = 30;
        auto& movement = _activeScene->AddComponent<Movement>(m);
        movement.velocity = {10.0f, 10.0f};
        movement.angularVelocity = 30.0f;
        auto& renderer = _activeScene->AddComponent<impala::SpriteRenderer>(m);
        renderer.sprite = asteroidSprite;
    }

    void DzoneGame::Update(impala::GameContext* context, float dt) {
        for (uint32_t i = 0; i < _players.PlayerCount(); ++i) {
            _players[i].Update(dt, _activeScene);
        }

        MovementSystem movementSystem;
        movementSystem.Update(dt, _activeScene, context);
    }

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
