#pragma once
#include <cstdint>

namespace impala {

    enum class ImpalaKey : uint16_t {
        KeyA = 4,
        KeyB = 5,
        KeyC = 6,
        KeyD = 7,
        KeyE = 8,
        KeyF = 9,
        KeyG = 10,
        KeyH = 11,
        KeyI = 12,
        KeyJ = 13,
        KeyK = 14,
        KeyL = 15,
        KeyM = 16,
        KeyN = 17,
        KeyO = 18,
        KeyP = 19,
        KeyQ = 20,
        KeyR = 21,
        KeyS = 22,
        KeyT = 23,
        KeyU = 24,
        KeyV = 25,
        KeyW = 26,
        KeyX = 27,
        KeyY = 28,
        KeyZ = 29,

        Key1 = 30,
        Key2 = 31,
        Key3 = 32,
        Key4 = 33,
        Key5 = 34,
        Key6 = 35,
        Key7 = 36,
        Key8 = 37,
        Key9 = 38,
        Key0 = 39,

        KeyEnter = 40,
        KeyEscape = 41,
        KeyBackspace = 42,
        KeyTab = 43,
        KeySpace = 44,

        KeyCapsLock = 57,

        KeyF1 = 58,
        KeyF2 = 59,
        KeyF3 = 60,
        KeyF4 = 61,
        KeyF5 = 62,
        KeyF6 = 63,
        KeyF7 = 64,
        KeyF8 = 65,
        KeyF9 = 66,
        KeyF10 = 67,
        KeyF11 = 68,
        KeyF12 = 69,

        KeyPrintScreen = 70,
        KeyScrollLock = 71,
        KeyPause = 72,
        KeyBreak = 72,

        KeyInsert = 73,
        KeyHome = 74,
        KeyPageUp = 75,
        KeyDelete = 76,
        KeyEnd = 77,
        KeyPageDown = 78,

        KeyRight = 79,
        KeyLeft = 80,
        KeyDown = 81,
        KeyUp = 82,

        KeyNumpadLock = 83,
        KeyNumpadDivide = 84,
        KeyNumpadMultiply = 85,
        KeyNumpadSubtract = 86,
        KeyNumpadAdd = 87,
        KeyNumpadEnter = 88,
        KeyNumpad1 = 89,
        KeyNumpad2 = 90,
        KeyNumpad3 = 91,
        KeyNumpad4 = 92,
        KeyNumpad5 = 93,
        KeyNumpad6 = 94,
        KeyNumpad7 = 95,
        KeyNumpad8 = 96,
        KeyNumpad9 = 97,
        KeyNumpad0 = 98,
        KeyNumpadDecimal = 99,

        KeyMenu = 118,

        KeyControl = 224,
        KeyShift = 225,
        KeyAlt = 226,
        KeyMeta = 227,
        KeyRightControl = 228,
        KeyRightShift = 229,
        KeyRightAlt = 230,
        KeyRightMeta = 231,

        KeyCount = 512
    };

    class IKeyboard {
    public:
        virtual ~IKeyboard() = default;
        virtual bool IsKeyPressed(ImpalaKey key) const = 0;
        virtual bool WasKeyPressedThisFrame(ImpalaKey key) const = 0;
        virtual bool WasKeyReleasedThisFrame(ImpalaKey key) const = 0;
    };

} // namespace impala
