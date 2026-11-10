#pragma once

#include "impala/keyboard.h"

namespace impala {

    class Keyboard : public IKeyboard {
    public:
        Keyboard() : _current(SDL_GetKeyboardState(nullptr)) {}
        bool IsKeyPressed(ImpalaKey key) const override;
        bool WasKeyPressedThisFrame(ImpalaKey key) const override;
        bool WasKeyReleasedThisFrame(ImpalaKey key) const override;

        void Update(float dt);
        void PostUpdate();

    private:
        static constexpr int IMPALA_KEY_COUNT = SDL_SCANCODE_COUNT;

        const bool* _current = nullptr;
        bool _previous[IMPALA_KEY_COUNT] = {false};
    };

} // namespace impala
