#include "gamepad_collection.h"
#include "gamepad.h"

namespace impala {

    void GamepadCollection::Update(float dt) {
        // TODO
    }

    void GamepadCollection::PostUpdate() {
        for (int i = 0; i < _gamepads.size(); ++i) {
            if (_gamepads[i].sdlGamepad) {
                _gamepads[i].gamepad.ClearCurrentState();
            }
        }
    }

    GamepadHandle GamepadCollection::GamepadConnected(SDL_JoystickID joystickId) {
        if (_gamepadMap.find(joystickId) != _gamepadMap.end()) {
            return _gamepadMap[joystickId];
        }

        auto sdlGamepad = SDL_OpenGamepad(joystickId);
        if (!sdlGamepad) {
            impala::log::Error(impala::log::LogCategory::Engine, "Failed to open gamepad: {} - {}",
                               std::to_string(joystickId), SDL_GetError());
            return InvalidGamepadHandle();
        }

        // Find an available slot or create a new one
        for (auto i = 0; i < _gamepads.size(); ++i) {
            if (_gamepads[i].sdlGamepad == nullptr) {
                GamepadHandle handle{static_cast<uint32_t>(i), _gamepads[i].generation};
                _gamepads[i].joystickId = joystickId;
                _gamepads[i].sdlGamepad = std::unique_ptr<SDL_Gamepad, detail::GamepadDeleter>(
                    sdlGamepad, detail::GamepadDeleter());
                _gamepadMap[joystickId] = handle;
                return handle;
            }
        }

        GamepadSlot newSlot;
        newSlot.joystickId = joystickId;
        newSlot.sdlGamepad = std::unique_ptr<SDL_Gamepad, detail::GamepadDeleter>(
            sdlGamepad, detail::GamepadDeleter());
        GamepadHandle handle{static_cast<uint32_t>(_gamepads.size()), newSlot.generation};
        _gamepads.push_back(std::move(newSlot));
        _gamepadMap[joystickId] = handle;
        return handle;
    }

    void GamepadCollection::GamepadDisconnected(SDL_JoystickID joystickId) {
        auto it = _gamepadMap.find(joystickId);
        if (it != _gamepadMap.end()) {
            GamepadHandle handle = it->second;
            _gamepadMap.erase(it);
            _gamepads[handle.id].sdlGamepad.reset();
            _gamepads[handle.id].gamepad.Reset();
            _gamepads[handle.id].generation++;
        } else {
            impala::log::Warn(impala::log::LogCategory::Engine,
                              "Gamepad disconnected not found: {}", std::to_string(joystickId));
        }
    }

    IGamepad* GamepadCollection::GetGamepad(GamepadHandle handle) {
        if (handle.id >= _gamepads.size()) {
            return nullptr;
        }

        if (_gamepads[handle.id].generation != handle.generation) {
            return nullptr;
        }

        return &_gamepads[handle.id].gamepad;
    }

} // namespace impala
