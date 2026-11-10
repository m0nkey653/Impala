#pragma once

#include <cstdint>

namespace impala {

    struct EntityHandle {
        uint32_t id = 0;
        uint32_t generation = 0;
        bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr EntityHandle InvalidEntityHandle() {
        return EntityHandle{UINT32_MAX, UINT32_MAX};
    }

    struct GamepadHandle {
        uint32_t id = 0;
        uint32_t generation = 0;
        bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr GamepadHandle InvalidGamepadHandle() {
        return GamepadHandle{UINT32_MAX, UINT32_MAX};
    }

    struct SpriteHandle {
        uint32_t id = 0;
        uint32_t generation = 0;
        constexpr bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr SpriteHandle InvalidSpriteHandle() {
        return SpriteHandle{UINT32_MAX, UINT32_MAX};
    }

    struct TextureHandle {
        uint32_t id = 0;
        uint32_t generation = 0;
        constexpr bool IsValid() const { return id != UINT32_MAX; }
    };

    constexpr TextureHandle InvalidTextureHandle() {
        return TextureHandle{UINT32_MAX, UINT32_MAX};
    }
} // namespace impala
