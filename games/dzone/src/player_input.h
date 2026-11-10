#pragma once

#include "impala/gamepad.h"
#include "impala/handles.h"
#include "impala/keyboard.h"
#include "impala/math.h"

namespace dzone {

    struct KeyboardInputMap {
        impala::ImpalaKey up;
        impala::ImpalaKey left;
        impala::ImpalaKey down;
        impala::ImpalaKey right;
    };

    constexpr KeyboardInputMap DEFAULT_KEYBOARD_INPUT_MAP_P1 =
        KeyboardInputMap{impala::ImpalaKey::KeyW, impala::ImpalaKey::KeyA, impala::ImpalaKey::KeyS,
                         impala::ImpalaKey::KeyD};

    constexpr KeyboardInputMap DEFAULT_KEYBOARD_INPUT_MAP_P2 =
        KeyboardInputMap{impala::ImpalaKey::KeyL, impala::ImpalaKey::KeyComma,
                         impala::ImpalaKey::KeyPeriod, impala::ImpalaKey::KeySlash};

    constexpr KeyboardInputMap DEFAULT_KEYBOARD_INPUT_MAP_P3 =
        KeyboardInputMap{impala::ImpalaKey::KeyUp, impala::ImpalaKey::KeyLeft,
                         impala::ImpalaKey::KeyDown, impala::ImpalaKey::KeyRight};

    struct GamepadInputMap {
        impala::GamepadAxis forward;
        impala::GamepadAxis rotate;
    };

    constexpr GamepadInputMap DEFAULT_GAMEPAD_INPUT_MAP =
        GamepadInputMap{impala::GamepadAxis::LeftY, impala::GamepadAxis::RightX};

    class PlayerInput {
    public:
        PlayerInput(impala::IKeyboard* keyboard, KeyboardInputMap keyboardInputMap,
                    impala::IGamepadCollection* gamepads)
            : _keyboard(keyboard), _keyboardInputMap(keyboardInputMap),
              _gamepadInputMap(DEFAULT_GAMEPAD_INPUT_MAP), _gamepads(gamepads) {}

        impala::GamepadHandle GetGamepadHandle() const { return _handle; }
        void SetGamepadHandle(impala::GamepadHandle handle) { _handle = handle; }

        float GetRotation() const;
        float GetThrust() const;

    private:
        impala::GamepadHandle _handle;
        impala::IKeyboard* _keyboard = nullptr;
        KeyboardInputMap _keyboardInputMap;
        GamepadInputMap _gamepadInputMap;
        impala::IGamepadCollection* _gamepads = nullptr;
    };

} // namespace dzone
