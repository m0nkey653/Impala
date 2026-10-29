#pragma once

#include <cstdint>

namespace impala {

    struct SpriteHandle {
        uint32_t id = 0;
        constexpr bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr SpriteHandle InvalidSpriteHandle() {
        return SpriteHandle{UINT32_MAX};
    }

    struct TextureHandle {
        uint32_t id = 0;
        constexpr bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr TextureHandle InvalidTextureHandle() {
        return TextureHandle{UINT32_MAX};
    }
} // namespace impala