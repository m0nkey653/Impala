#pragma once

#include "impala/handles.h"

namespace dzone {

    class PlayerInput {
    public:
        void Update(float dt);

        impala::GamepadHandle GetGamepadHandle() const { return _handle; }
        void SetGamepadHandle(impala::GamepadHandle handle) { _handle = handle; }

    private:
        impala::GamepadHandle _handle = impala::InvalidGamepadHandle();
    };

} // namespace dzone
