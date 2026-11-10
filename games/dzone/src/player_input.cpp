#include "player_input.h"

namespace dzone {

    float PlayerInput::GetRotation() const {
        // Controller takes precedent over keyboard
        auto gamepad = _gamepads->GetGamepad(_handle);
        if (gamepad) {
            return gamepad->GetAxis(_gamepadInputMap.rotate);
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
            return gamepad->GetAxis(_gamepadInputMap.forward);
        } else {
            // if both keys are pressed - cancel them out
            float thrust = 0.0f;
            _keyboard->IsKeyPressed(_keyboardInputMap.up) ? thrust += 1.0f : 0;
            _keyboard->IsKeyPressed(_keyboardInputMap.down) ? thrust -= 1.0f : 0;
            return thrust;
        }
    }

} // namespace dzone
