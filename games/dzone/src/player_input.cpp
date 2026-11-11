#include "player_input.h"

namespace dzone {

    constexpr float DEADZONE = 0.08f;

    float PlayerInput::GetRotation() const {
        // Controller takes precedent over keyboard
        auto gamepad = _gamepads->GetGamepad(_handle);
        if (gamepad) {
            auto axisValue = gamepad->GetAxis(_gamepadInputMap.rotate);
            if (std::abs(axisValue) < DEADZONE) {
                return 0.0f;
            }
            return axisValue;
        } else {
            // if both keys are pressed - cancel them out
            float rotation = 0.0f;
            _keyboard->IsKeyPressed(_keyboardInputMap.left) ? rotation -= 1.0f : 0;
            _keyboard->IsKeyPressed(_keyboardInputMap.right) ? rotation += 1.0f : 0;
            return rotation;
        }
    }

    float PlayerInput::GetThrust() const {
        // Controller takes precedent over keyboard
        auto gamepad = _gamepads->GetGamepad(_handle);
        if (gamepad) {
            // up on the gamepad is negative, so invert it to match screen coordinates
            auto axisValue = gamepad->GetAxis(_gamepadInputMap.forward);
            if (std::abs(axisValue) < DEADZONE) {
                return 0.0f;
            }
            return -1 * axisValue;
        } else {
            // if both keys are pressed - cancel them out
            float thrust = 0.0f;
            _keyboard->IsKeyPressed(_keyboardInputMap.up) ? thrust += 1.0f : 0;
            _keyboard->IsKeyPressed(_keyboardInputMap.down) ? thrust -= 1.0f : 0;
            return thrust;
        }
    }

} // namespace dzone
