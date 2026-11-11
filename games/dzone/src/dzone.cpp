#include "dzone.h"
#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/log.h"

namespace dzone {

    void DzoneGame::Init(impala::GameContext* context) {

        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas(dzone::KENNEY_SPACE_SHOOTER_SPRITE_ATLAS);
        auto playerSprite0 = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_1_BLUE);
        auto playerSprite1 = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_2_GREEN);
        assetDataBase->LoadSpriteTexture(playerSprite0);
        assetDataBase->LoadSpriteTexture(playerSprite1);

        _activeScene = context->sceneManager->CreateScene();
        _players.AddPlayer("Player 1", context, _activeScene, playerSprite0);
        _players.AddPlayer("Player 2", context, _activeScene, playerSprite1);
    }

    void DzoneGame::Update(impala::GameContext* context, float dt) {
        for (uint32_t i = 0; i < _players.PlayerCount(); ++i) {
            _players[i].Update(dt, _activeScene);
        }
    }

    void DzoneGame::Render(impala::GameContext* context) {
        // Render game here
    }

    void DzoneGame::OnEvent(const impala::GameEvent& event) {
        // Handle game events here
    }

    void DzoneGame::OnGamepadConnected(impala::GamepadHandle handle) {
        _players.AddGamepadToPlayer(handle);
    }

    void DzoneGame::OnGamepadDisconnected(impala::GamepadHandle handle) {
        impala::log::Debug(impala::log::LogCategory::Game, "Gamepad connected: {}", handle);
        // Handle gamepad disconnection here
        _players.RemoveGamepadFromPlayer(handle);
    }

    void DzoneGame::Shutdown() {
        // Cleanup code here
    }

} // namespace dzone
