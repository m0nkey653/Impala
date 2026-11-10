#include "gamepad_collection.h"
#include "gamepad.h"

namespace impala {

    void GamepadCollection::Update(float dt) {
        for (auto& slot : _gamepads) {
            if (slot.sdlGamepad) {
                GamepadState state;
                state.axisStates[static_cast<uint8_t>(GamepadAxis::LeftX)] =
                    SDL_GetGamepadAxis(slot.sdlGamepad.get(), SDL_GAMEPAD_AXIS_LEFTX);
                state.axisStates[static_cast<uint8_t>(GamepadAxis::LeftY)] =
                    SDL_GetGamepadAxis(slot.sdlGamepad.get(), SDL_GAMEPAD_AXIS_LEFTY);
                state.axisStates[static_cast<uint8_t>(GamepadAxis::RightX)] =
                    SDL_GetGamepadAxis(slot.sdlGamepad.get(), SDL_GAMEPAD_AXIS_RIGHTX);
                state.axisStates[static_cast<uint8_t>(GamepadAxis::RightY)] =
                    SDL_GetGamepadAxis(slot.sdlGamepad.get(), SDL_GAMEPAD_AXIS_RIGHTY);
                state.axisStates[static_cast<uint8_t>(GamepadAxis::LeftTrigger)] =
                    SDL_GetGamepadAxis(slot.sdlGamepad.get(), SDL_GAMEPAD_AXIS_LEFT_TRIGGER);
                state.axisStates[static_cast<uint8_t>(GamepadAxis::RightTrigger)] =
                    SDL_GetGamepadAxis(slot.sdlGamepad.get(), SDL_GAMEPAD_AXIS_RIGHT_TRIGGER);

                state.buttonStates[static_cast<uint8_t>(GamepadButton::South)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_SOUTH);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::East)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_EAST);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::West)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_WEST);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::North)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_NORTH);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::LeftShoulder)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_LEFT_SHOULDER);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::RightShoulder)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_RIGHT_SHOULDER);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::Start)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_START);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::Back)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_BACK);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::LeftStick)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_LEFT_STICK);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::RightStick)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_RIGHT_STICK);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::DPadUp)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_DPAD_UP);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::DPadDown)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_DPAD_DOWN);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::DPadLeft)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_DPAD_LEFT);
                state.buttonStates[static_cast<uint8_t>(GamepadButton::DPadRight)] =
                    SDL_GetGamepadButton(slot.sdlGamepad.get(), SDL_GAMEPAD_BUTTON_DPAD_RIGHT);
                slot.gamepad.Update(state);
            }
        }
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
            impala::log::Error(impala::log::LogCategory::Input, "Failed to open gamepad: {} - {}",
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
                impala::log::Info(impala::log::LogCategory::Input,
                                  "Gamepad connected: Handle {} - {}", handle.id, joystickId);
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
        impala::log::Info(impala::log::LogCategory::Input, "Gamepad connected: Handle {} - {}",
                          handle.id, joystickId);
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
            impala::log::Warn(impala::log::LogCategory::Input, "Gamepad disconnected not found: {}",
                              std::to_string(joystickId));
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

    std::span<GamepadHandle> GamepadCollection::GetConnectedGamepads() {
        return std::span<GamepadHandle>(reinterpret_cast<GamepadHandle*>(_gamepads.data()),
                                        _gamepads.size());
    }

} // namespace impala
