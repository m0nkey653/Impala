#pragma once

#include "impala/gamepad.h"
#include "impala/handles.h"
#include "impala/keyboard.h"

namespace dzone {

    struct KeyboardInputMap {
        impala::ImpalaKey up = impala::ImpalaKey::KeyCount;
        impala::ImpalaKey left = impala::ImpalaKey::KeyCount;
        impala::ImpalaKey down = impala::ImpalaKey::KeyCount;
        impala::ImpalaKey right = impala::ImpalaKey::KeyCount;
        impala::ImpalaKey firePrimary = impala::ImpalaKey::KeyCount;
        impala::ImpalaKey fireSecondary = impala::ImpalaKey::KeyCount;

        bool IsActive(impala::IKeyboard* keyboard) const {
            // just check if the first key is set
            // players 1,2,3 can use keyboard
            // players 4-8 need additional gamepads
            return up != impala::ImpalaKey::KeyCount;
        }
    };

    constexpr KeyboardInputMap DEFAULT_KEYBOARD_INPUT_MAP_P1 =
        KeyboardInputMap{impala::ImpalaKey::KeyW, impala::ImpalaKey::KeyA, impala::ImpalaKey::KeyS,
                         impala::ImpalaKey::KeyD, impala::ImpalaKey::KeySpace};

    constexpr KeyboardInputMap DEFAULT_KEYBOARD_INPUT_MAP_P2 = KeyboardInputMap{
        impala::ImpalaKey::KeyL, impala::ImpalaKey::KeyComma, impala::ImpalaKey::KeyPeriod,
        impala::ImpalaKey::KeySlash, impala::ImpalaKey::KeyEnter};

    constexpr KeyboardInputMap DEFAULT_KEYBOARD_INPUT_MAP_P3 = KeyboardInputMap{
        impala::ImpalaKey::KeyUp, impala::ImpalaKey::KeyLeft, impala::ImpalaKey::KeyDown,
        impala::ImpalaKey::KeyRight, impala::ImpalaKey::KeyNumpadEnter};

    constexpr KeyboardInputMap UNUSED_KEYBOARD_INPUT_MAP = KeyboardInputMap{
        impala::ImpalaKey::KeyCount, impala::ImpalaKey::KeyCount, impala::ImpalaKey::KeyCount,
        impala::ImpalaKey::KeyCount, impala::ImpalaKey::KeyCount};

    constexpr KeyboardInputMap KEYBOARD_MAPS[8]{
        DEFAULT_KEYBOARD_INPUT_MAP_P1, DEFAULT_KEYBOARD_INPUT_MAP_P2, DEFAULT_KEYBOARD_INPUT_MAP_P3,
        UNUSED_KEYBOARD_INPUT_MAP,     UNUSED_KEYBOARD_INPUT_MAP,     UNUSED_KEYBOARD_INPUT_MAP,
        UNUSED_KEYBOARD_INPUT_MAP,     UNUSED_KEYBOARD_INPUT_MAP};

    struct GamepadInputMap {
        impala::GamepadAxis forward;
        impala::GamepadAxis rotate;
        impala::GamepadAxis firePrimary;
        impala::GamepadAxis fireSecondary;
    };

    constexpr GamepadInputMap DEFAULT_GAMEPAD_INPUT_MAP =
        GamepadInputMap{impala::GamepadAxis::LeftY, impala::GamepadAxis::RightX,
                        impala::GamepadAxis::RightTrigger, impala::GamepadAxis::LeftTrigger};

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
        bool GetFirePrimary() const;
        bool GetFireSecondary() const;

    private:
        impala::GamepadHandle _handle;
        impala::IKeyboard* _keyboard = nullptr;
        KeyboardInputMap _keyboardInputMap;
        GamepadInputMap _gamepadInputMap;
        impala::IGamepadCollection* _gamepads = nullptr;
    };

} // namespace dzone
