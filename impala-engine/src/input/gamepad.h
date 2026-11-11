#pragma once
#include "impala/gamepad.h"

namespace impala {

    struct GamepadState {
        bool buttonStates[16] = {false};
        float axisStates[6] = {0.0f};
    };

    class Gamepad : public IGamepad {
    public:
        bool IsButtonPressed(GamepadButton button) const override;
        float GetAxis(GamepadAxis axis) const override;
        float GetAxisLastFrame(GamepadAxis axis) const override;

        bool WasButtonPressedThisFrame(GamepadButton button) const override;
        bool WasButtonReleasedThisFrame(GamepadButton button) const override;

        void Update(GamepadState state);
        void Reset();
        void ClearCurrentState();

    private:
        // Controller state variables
        GamepadState _current = {};
        GamepadState _previous = {};
    };

} // namespace impala
