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
        float GetAxisLastFixed(GamepadAxis axis) const override;

        bool WasButtonPressedThisFrame(GamepadButton button) const override;
        bool WasButtonReleasedThisFrame(GamepadButton button) const override;

        bool WasButtonPressedThisFixed(GamepadButton button) const override;
        bool WasButtonReleasedThisFixed(GamepadButton button) const override;

        void Update(GamepadState state);
        void Reset();
        void ClearCurrentFrameState();
        void ClearCurrentFixedState();

    private:
        // Controller state variables
        GamepadState _current = {};
        GamepadState _previousFrame = {};
        GamepadState _previousFixed = {};
    };

} // namespace impala
