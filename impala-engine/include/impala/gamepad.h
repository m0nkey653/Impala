#pragma once
#include "handles.h"
#include <span>

namespace impala {

    enum class GamepadButton {
        South,
        East,
        West,
        North,
        LeftShoulder,
        RightShoulder,
        Start,
        Back,
        LeftStick,
        RightStick,
        DPadUp,
        DPadDown,
        DPadLeft,
        DPadRight
    };

    enum class GamepadAxis { LeftX, LeftY, RightX, RightY, LeftTrigger, RightTrigger };

    class IGamepad {
    public:
        virtual ~IGamepad() = default;

        virtual bool IsButtonPressed(GamepadButton button) const = 0;
        virtual float GetAxis(GamepadAxis axis) const = 0;
        virtual bool WasButtonPressedThisFrame(GamepadButton button) const = 0;
        virtual bool WasButtonReleasedThisFrame(GamepadButton button) const = 0;
    };

    class IGamepadCollection {
    public:
        virtual ~IGamepadCollection() = default;
        virtual IGamepad* GetGamepad(GamepadHandle handle) = 0;
        virtual std::span<GamepadHandle> GetConnectedGamepads() = 0;
    };

} // namespace impala
