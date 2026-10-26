#pragma once
#include "sprite_rect.h"
#include "texture.h"

namespace impala {
    struct ImpalaSprite {
        SpriteRect rect;
        Texture texture;
    };
} // namespace impala