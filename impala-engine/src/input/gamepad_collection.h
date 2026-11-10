#pragma once
#include "gamepad.h"

namespace impala {

    namespace detail {
        struct GamepadDeleter {
            void operator()(SDL_Gamepad* gamepad) const { SDL_CloseGamepad(gamepad); }
        };
    } // namespace detail

    struct GamepadSlot {
        uint32_t generation = 0;
        SDL_JoystickID joystickId;
        std::unique_ptr<SDL_Gamepad, detail::GamepadDeleter> sdlGamepad = nullptr;
        Gamepad gamepad = {};
    };

    class GamepadCollection : public IGamepadCollection {
    public:
        GamepadCollection() = default;
        void Update(float dt);
        void PostUpdate();

        GamepadHandle GamepadConnected(SDL_JoystickID joystickId);
        void GamepadDisconnected(SDL_JoystickID joystickId);

        IGamepad* GetGamepad(GamepadHandle handle) override;
        std::vector<GamepadHandle> GetConnectedGamepads() override;

    private:
        std::vector<GamepadSlot> _gamepads;
        std::unordered_map<SDL_JoystickID, GamepadHandle> _gamepadMap;
    };
} // namespace impala
