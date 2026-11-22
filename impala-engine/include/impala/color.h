#pragma once

namespace impala {

    struct Color {
        float r = 0, g = 0, b = 0, a = 1;
    };

    constexpr bool operator==(const Color& lhs, const Color& rhs) {
        return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b && lhs.a == rhs.a;
    }
    constexpr bool operator!=(const Color& lhs, const Color& rhs) {
        return !(lhs == rhs);
    }

} // namespace impala

namespace impala::color {
    constexpr Color White() {
        return {1, 1, 1, 1};
    }
    constexpr Color Black() {
        return {0, 0, 0, 1};
    }
    constexpr Color Red() {
        return {1, 0, 0, 1};
    }
    constexpr Color Green() {
        return {0, 1, 0, 1};
    }
    constexpr Color Blue() {
        return {0, 0, 1, 1};
    }
    constexpr Color Transparent() {
        return {0, 0, 0, 0};
    }
    constexpr Color Yellow() {
        return {1, 1, 0, 1};
    }
    constexpr Color Magenta() {
        return {1, 0, 1, 1};
    }
    constexpr Color Cyan() {
        return {0, 1, 1, 1};
    }
    constexpr Color Orange() {
        return {1, 0.5, 0, 1};
    }
    constexpr Color Purple() {
        return {0.5, 0, 0.5, 1};
    }
    constexpr Color Brown() {
        return {0.6, 0.3, 0, 1};
    }
    constexpr Color Gray() {
        return {0.5, 0.5, 0.5, 1};
    }
    constexpr Color LightGray() {
        return {0.75, 0.75, 0.75, 1};
    }
    constexpr Color Pink() {
        return {1, 0.75, 0.8, 1};
    }
    constexpr Color LightBlue() {
        return {0.68, 0.85, 0.9, 1};
    }
    constexpr Color LightGreen() {
        return {0.56, 0.93, 0.56, 1};
    }
    constexpr Color LightYellow() {
        return {1, 1, 0.88, 1};
    }
    constexpr Color LightPink() {
        return {1, 0.71, 0.76, 1};
    }
    constexpr Color LightOrange() {
        return {1, 0.8, 0.6, 1};
    }
    constexpr Color LightPurple() {
        return {0.8, 0.6, 0.8, 1};
    }
    constexpr Color LightBrown() {
        return {0.8, 0.6, 0.4, 1};
    }
    constexpr Color DarkBrown() {
        return {0.4, 0.2, 0, 1};
    }
    constexpr Color DarkGray() {
        return {0.25, 0.25, 0.25, 1};
    }
    constexpr Color DarkBlue() {
        return {0, 0, 0.5, 1};
    }
    constexpr Color DarkRed() {
        return {0.5, 0, 0, 1};
    }
    constexpr Color DarkGreen() {
        return {0, 0.5, 0, 1};
    }
    constexpr Color DarkCyan() {
        return {0, 0.5, 0.5, 1};
    }
    constexpr Color DarkMagenta() {
        return {0.5, 0, 0.5, 1};
    }
    constexpr Color DarkYellow() {
        return {0.5, 0.5, 0, 1};
    }
    constexpr Color DarkOrange() {
        return {0.8, 0.4, 0, 1};
    }
    constexpr Color DarkPurple() {
        return {0.4, 0, 0.4, 1};
    }
    constexpr Color DarkPink() {
        return {0.8, 0.4, 0.5, 1};
    }
} // namespace impala::color
