#pragma once

#include "impala/keyboard.h"

namespace impala {

    struct KeyboardState {
        bool keyStates[static_cast<int>(SDL_SCANCODE_COUNT)] = {false};
    };

    class Keyboard : public IKeyboard {
    public:
        bool IsKeyPressed(ImpalaKey key) const override;
        bool WasKeyPressedThisFrame(ImpalaKey key) const override;
        bool WasKeyReleasedThisFrame(ImpalaKey key) const override;

        void Update(float dt);
        void PostUpdate();

        SDL_Scancode GetSdlKey(ImpalaKey key) const;

    private:
        KeyboardState _currentKeyStates;
        KeyboardState _previousKeyStates;
    };

} // namespace impala
