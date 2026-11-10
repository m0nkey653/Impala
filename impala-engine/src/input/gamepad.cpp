#include "gamepad.h"

namespace impala {

    void Gamepad::Update(GamepadState state) {
        _current = state;
    }

    void Gamepad::Reset() {
        _current = GamepadState{};
        _previous = GamepadState{};
    }

    bool Gamepad::IsButtonPressed(GamepadButton button) const {
        // Return the state of the specified button
        return _current.buttonStates[static_cast<int>(button)];
    }

    bool Gamepad::WasButtonPressedThisFrame(GamepadButton button) const {
        // Return true if the button was not pressed in the previous state but is pressed in the
        // current state
        return _current.buttonStates[static_cast<int>(button)] &&
               !_previous.buttonStates[static_cast<int>(button)];
    }

    bool Gamepad::WasButtonReleasedThisFrame(GamepadButton button) const {
        // Return true if the button was pressed in the previous state but is not pressed in the
        // current state
        return !_current.buttonStates[static_cast<int>(button)] &&
               _previous.buttonStates[static_cast<int>(button)];
    }

    float Gamepad::GetAxis(GamepadAxis axis) const {
        // Return the value of the specified axis

        // LEFTX, RIGHTX	about 0	-32768 (full left) to 32767 (full right)
        // LEFTY, RIGHTY	about 0	-32768 (full up) to 32767 (full down)	Positive is down.
        // LEFT_TRIGGER, RIGHT_TRIGGER	0	0 (released) to 32767 (fully pressed)	Never
        // negative.

        auto result = _current.axisStates[static_cast<int>(axis)];
        switch (axis) {
        case GamepadAxis::LeftX:
        case GamepadAxis::RightX:
        case GamepadAxis::LeftY:
        case GamepadAxis::RightY:
            return std::max(result /= 32767.0f, -1.0f);
        case GamepadAxis::LeftTrigger:
        case GamepadAxis::RightTrigger:
            return result > 0;
        }

        return result;
    }

    void Gamepad::ClearCurrentState() {
        _previous = _current;
        _current = GamepadState{};
    }

} // namespace impala
