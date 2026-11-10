#include "gamepad.h"
#include <cstdint>

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
        return _current.axisStates[static_cast<int>(axis)];
    }

    void Gamepad::ClearCurrentState() {
        _previous = _current;
        _current = GamepadState{};
    }

} // namespace impala
