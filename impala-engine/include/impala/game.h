#pragma once

#include "impala/asset_database.h"
#include "impala/gamepad.h"
#include "impala/keyboard.h"
#include "impala/scene_manager.h"

namespace impala {

    class IGameRenderer2D {
    public:
        virtual ~IGameRenderer2D() = default;
    };

    class IGameRenderer3D {
    public:
        virtual ~IGameRenderer3D() = default;
    };

    typedef union GameEvent {

    } GameEvent;

    struct GameContext {
        ISceneManager* sceneManager;
        IGameRenderer2D* renderer2D;
        IGameRenderer3D* renderer3D;
        IAssetDataBase* assetDataBase;
        IGamepadCollection* gamepads;
        IKeyboard* keyboard;
    };

    struct GameInfo {
        const char* name;
        const char* version;
        const char* iconPath;
    };

    class IGame {
    public:
        virtual ~IGame() = default;
        virtual GameInfo GetGameInfo() const = 0;
        virtual void Init(GameContext* context) = 0;
        virtual void Update(GameContext* context, float dt) = 0;
        virtual void Render(GameContext* context) = 0;
        virtual void OnEvent(const GameEvent& event) = 0;

        virtual void OnGamepadConnected(GamepadHandle handle) {}
        virtual void OnGamepadDisconnected(GamepadHandle handle) {}

        virtual void Shutdown() = 0;
    };

    IGame* CreateGame();
} // namespace impala
