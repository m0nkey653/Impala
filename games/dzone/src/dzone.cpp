#include "dzone.h"
#include "dzone_assets.h"
#include "impala/game.h"
#include "impala/log.h"
#include "impala/sprite.h"
#include "impala/transform.h"
#include "player_input.h"

namespace dzone {

    void DzoneGame::Init(impala::GameContext* context) {

        auto* assetDataBase = context->assetDataBase;
        assetDataBase->AddSpriteAtlas(dzone::KENNEY_SPACE_SHOOTER_SPRITE_ATLAS);
        auto playerSprite0 = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_1_BLUE);
        auto playerSprite1 = assetDataBase->GetSprite(dzone::KENNEY_SPACE_SHOOTER_SHIP_2_GREEN);
        assetDataBase->LoadSpriteTexture(playerSprite0);
        assetDataBase->LoadSpriteTexture(playerSprite1);

        _activeScene = context->sceneManager->CreateScene();

        auto entityP0 = _activeScene->CreateEntity("Player 1");
        _activeScene->AddComponent<impala::Transform2d>(entityP0);
        auto transform = _activeScene->GetComponent<impala::Transform2d>(entityP0);
        transform->position = {100.0f, 100.0f}; // Set initial position for the player
        transform->rotation = 90;
        _activeScene->AddComponent<impala::SpriteRenderer>(entityP0);
        auto spriteRenderer = _activeScene->GetComponent<impala::SpriteRenderer>(entityP0);
        spriteRenderer->sprite = playerSprite0;
        PlayerInput input0(context->keyboard, dzone::DEFAULT_KEYBOARD_INPUT_MAP_P1,
                           context->gamepads);
        impala::log::Debug(impala::log::LogCategory::Game, "Created player 1 entity with ID {}",
                           entityP0.id);
        _players.push_back(DzonePlayer(entityP0, input0));

        auto entityP1 = _activeScene->CreateEntity("Player 2");
        _activeScene->AddComponent<impala::Transform2d>(entityP1);
        auto transform1 = _activeScene->GetComponent<impala::Transform2d>(entityP1);
        transform1->position = {400.0f, 100.0f}; // Set initial position for the player
        transform1->rotation = -90;
        _activeScene->AddComponent<impala::SpriteRenderer>(entityP1);
        auto spriteRenderer1 = _activeScene->GetComponent<impala::SpriteRenderer>(entityP1);
        spriteRenderer1->sprite = playerSprite1;
        PlayerInput input1(context->keyboard, dzone::DEFAULT_KEYBOARD_INPUT_MAP_P2,
                           context->gamepads);
        impala::log::Debug(impala::log::LogCategory::Game, "Created player 2 entity with ID {}",
                           entityP1.id);
        _players.push_back(DzonePlayer(entityP1, input1));
    }

    void DzoneGame::Update(impala::GameContext* context, float dt) {
        for (auto& player : _players) {
            player.Update(dt, _activeScene);
        }
    }

    void DzoneGame::Render(impala::GameContext* context) {
        // Render game here
    }

    void DzoneGame::OnEvent(const impala::GameEvent& event) {
        // Handle game events here
    }

    void DzoneGame::OnGamepadConnected(impala::GamepadHandle handle) {
        impala::log::Debug(impala::log::LogCategory::Game, "Gamepad connected: {}", handle);
        if (_gamepadToPlayerMap.find(handle) == _gamepadToPlayerMap.end()) {
            // Assign the gamepad to the first available player
            for (int i = 0; i < _players.size(); ++i) {
                if (std::find_if(_gamepadToPlayerMap.begin(), _gamepadToPlayerMap.end(),
                                 [i](const auto& pair) { return pair.second == i; }) ==
                    _gamepadToPlayerMap.end()) {
                    _gamepadToPlayerMap[handle] = i;
                    impala::log::Debug(impala::log::LogCategory::Game,
                                       "Assigned gamepad {} to player {}", handle, i);
                    _players[i].SetGamepad(handle);
                    break;
                }
            }
        }
    }

    void DzoneGame::OnGamepadDisconnected(impala::GamepadHandle handle) {
        impala::log::Debug(impala::log::LogCategory::Game, "Gamepad connected: {}", handle);
        // Handle gamepad disconnection here
        auto it = _gamepadToPlayerMap.find(handle);
        if (it != _gamepadToPlayerMap.end()) {
            _players[it->second].SetGamepad({});
            _gamepadToPlayerMap.erase(it);
        }
    }

    void DzoneGame::Shutdown() {
        // Cleanup code here
    }

} // namespace dzone
