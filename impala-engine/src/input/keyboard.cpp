#include "keyboard.h"

// ImpalaKey mirrors SDL_Scancode (USB HID usage codes)
static_assert(static_cast<int>(impala::ImpalaKey::KeyA) == SDL_SCANCODE_A);
static_assert(static_cast<int>(impala::ImpalaKey::KeyZ) == SDL_SCANCODE_Z);
static_assert(static_cast<int>(impala::ImpalaKey::Key1) == SDL_SCANCODE_1);
static_assert(static_cast<int>(impala::ImpalaKey::Key0) == SDL_SCANCODE_0);
static_assert(static_cast<int>(impala::ImpalaKey::KeyEnter) == SDL_SCANCODE_RETURN);
static_assert(static_cast<int>(impala::ImpalaKey::KeySpace) == SDL_SCANCODE_SPACE);
static_assert(static_cast<int>(impala::ImpalaKey::KeyCapsLock) == SDL_SCANCODE_CAPSLOCK);
static_assert(static_cast<int>(impala::ImpalaKey::KeyF1) == SDL_SCANCODE_F1);
static_assert(static_cast<int>(impala::ImpalaKey::KeyF12) == SDL_SCANCODE_F12);
static_assert(static_cast<int>(impala::ImpalaKey::KeyPrintScreen) == SDL_SCANCODE_PRINTSCREEN);
static_assert(static_cast<int>(impala::ImpalaKey::KeyPause) == SDL_SCANCODE_PAUSE);
static_assert(static_cast<int>(impala::ImpalaKey::KeyInsert) == SDL_SCANCODE_INSERT);
static_assert(static_cast<int>(impala::ImpalaKey::KeyPageDown) == SDL_SCANCODE_PAGEDOWN);
static_assert(static_cast<int>(impala::ImpalaKey::KeyRight) == SDL_SCANCODE_RIGHT);
static_assert(static_cast<int>(impala::ImpalaKey::KeyUp) == SDL_SCANCODE_UP);
static_assert(static_cast<int>(impala::ImpalaKey::KeyNumpadLock) == SDL_SCANCODE_NUMLOCKCLEAR);
static_assert(static_cast<int>(impala::ImpalaKey::KeyNumpad1) == SDL_SCANCODE_KP_1);
static_assert(static_cast<int>(impala::ImpalaKey::KeyNumpad0) == SDL_SCANCODE_KP_0);
static_assert(static_cast<int>(impala::ImpalaKey::KeyMenu) == SDL_SCANCODE_MENU);
static_assert(static_cast<int>(impala::ImpalaKey::KeyNumpadDecimal) == SDL_SCANCODE_KP_PERIOD);
static_assert(static_cast<int>(impala::ImpalaKey::KeyControl) == SDL_SCANCODE_LCTRL);
static_assert(static_cast<int>(impala::ImpalaKey::KeyMeta) == SDL_SCANCODE_LGUI);
static_assert(static_cast<int>(impala::ImpalaKey::KeyRightControl) == SDL_SCANCODE_RCTRL);
static_assert(static_cast<int>(impala::ImpalaKey::KeyRightMeta) == SDL_SCANCODE_RGUI);
static_assert(static_cast<int>(impala::ImpalaKey::KeyCount) == SDL_SCANCODE_COUNT);

namespace impala {

    bool Keyboard::IsKeyPressed(ImpalaKey key) const {
        int index = static_cast<int>(key);
        return _current[index];
    }

    bool Keyboard::WasKeyPressedThisFrame(ImpalaKey key) const {
        int index = static_cast<int>(key);
        return _current[index] && !_previous[index];
    }

    bool Keyboard::WasKeyReleasedThisFrame(ImpalaKey key) const {
        int index = static_cast<int>(key);
        return !_current[index] && _previous[index];
    }

    void Keyboard::PostUpdate() {
        std::copy_n(_current, IMPALA_KEY_COUNT, _previous);
    }

} // namespace impala
