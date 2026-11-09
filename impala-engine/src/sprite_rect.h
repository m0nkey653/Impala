#pragma once

namespace impala {
    struct SpriteRect {
        uint32_t id;
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
        SDL_Rect ToSDLRect() const {
            return SDL_Rect{static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
                            static_cast<int>(height)};
        }
        SDL_FRect ToSDLFRect() const {
            return SDL_FRect{static_cast<float>(x), static_cast<float>(y),
                             static_cast<float>(width), static_cast<float>(height)};
        }
    };
} // namespace impala