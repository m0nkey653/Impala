#pragma once
#include "impala/handles.h"
#include "sprite_rect.h"

namespace impala {
    struct ImpalaSprite {
        SpriteRect rect;
        TextureHandle textureHandle;
    };

    constexpr ImpalaSprite InvalidSprite() {
        return {{0, 0, 0, 0}, InvalidTextureHandle()};
    }

} // namespace impala